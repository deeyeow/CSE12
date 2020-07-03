/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/9/2020
                                                        cs12wi20fi
                                Assignment Six

File Name:      Hash.c
Description:    This program allows variable assignment to the calculator using
		a Symbol Table, that we will implement with a custom hash table.
		Our hash table will be double hashing using probe count array to
		track insertion attempts. This program will also use the
		FairShare algorithm to resolve any collisions.

******************************************************************************/
#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0;
int HashTable :: counter = 0;

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
        if (!option)
	{
		debug = false;
	}
	else
	{
		debug = true;
	}
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates an initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{

        /* initialize all table elements */
	for (int i = 0; i < size; i++)
	{
		table[i] = NULL;
	}

}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{

	/* call function to delete individual elements */
	for (int i = 0; i < size; i++)
	{
		delete table[i];
		table[i] = NULL;
	}

	/* delete table itself */
	delete[] table;
	table = NULL;

	/* delete probeCount itself */
	delete[] probeCount;
	probeCount = NULL;

	if (debug)
	{
		cerr << HASH << table_count << DEBUG_DEALLOCATE << endl;
	}
}	/* end: ~HashTable */


bool HashTable :: Insert (Base * element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether or not we are entering from recursion
% <return>           true or false indicating success or failure of insertion
***************************************************************************/

{

	/* set initial count */
	count = initialCount;

	Base *tempElement;
	int tempCount;

	/* check if already probed entire hash table for valid or empty space */
	if (count >= size)
	{
		return false;
	}

	/* if element not located, which means can insert */
	if (!Locate(element))
	{
		/* index is empty, insert now */
		if (table[index] == NULL)
		{
			if (debug)
			{
				cerr << HASH << table_count << INSERT <<
				element << "]" << endl;
			}

			table[index] = element;
			probeCount[index] = count;

			/* item is added, so increment occupancy */
			occupancy++;
		}

		/* index is occupied by another element, recurse */
		else
		{
			if (debug)
			{
				cerr << BUMP << endl;
			}

			/* store location values */
			tempElement = table[index];
			tempCount = probeCount[index];

			/* put element into location */
			table[index] = element;
			probeCount[index] = count;

			/* recurse with element in previous location */
			Insert(tempElement, ++tempCount);
		}
	}

	/* element located, which means there is duplicate */
	else
	{
		if (debug)
		{
			cerr << HASH << table_count << INSERT << element <<
			"]" << endl;
		}

		/* put element into location */
		table[index] = element;
		probeCount[index] = count;

		/* duplicate is replaced, so no need to increment occupancy */
	}

	return true;
}


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if item found, or false if not found
**********************************************************************/

{

	/* set hashcode, initial hashing location, and increment */
	long hashcode = *element;
	int initial = hashcode % size;
	int increment = hashcode  % (size - 1) + 1;

	/* use count to quickly locate index, instead of starting at the
	beginning */
	index = (initial + (count - 1) * increment) % size;

	/* print debug msg */
	if (debug)
	{
		cerr << HASH << table_count << DEBUG_LOCATE << endl;
		cerr << PROCESSING << (char *) (*element) << "]" << endl;
		cerr << HASH_VAL << hashcode << "]" << endl;
	}

	/* while hash table is not full */
	while (occupancy < size)
	{
		if (debug)
		{
			cerr << TRYING << index << "]" << endl;
		}

		/* check if index is empty, which means not found */
		if (table[index] == NULL)
		{
			if (debug)
			{
				cerr << HASH << table_count << FOUND_SPOT 
				<< endl;
			}

			return false;
		}

		/* if not null, occupied by another element */
		else
		{
			if (debug)
			{
				cerr << HASH << table_count << COMPARE <<
				table[index] << AND << element << "]" << endl;
			}

			/* check if element is duplicate */
			if (*table[index] == *element)
			{
				return true;
			}

			/* if not duplicate, location is occupied by different
			 * element */
			 else
			 {
				/* found index to replace using FairShare
				 * algorithm, which means element not found */
				if (probeCount[index] < count)
				{
					return false;
				}
			 }
		}

		/* increment number of locations checked */
		count++;

		/* compute next index to check */
		index = (index + increment) % size;
	}

	/* hash table is full, can't perform any more operations */
	return true;
}

Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
	/* setting count */
	count = 1;

	if (debug)
	{
		cerr << HASH + table_count << DEBUG_LOOKUP << endl;
	}

	/* if locate returns true, element has been found */
	if (Locate(element))
	{
		return table[index];
	}

	/* else, element not found and return null */
	return NULL;
}


ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
