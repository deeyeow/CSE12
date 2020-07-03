/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/9/2020
                                                        cs12wi20fi
                                Assignment Six

File Name:      Driver.c
Description:    The driver file for testing our hash table. Allocates,
		deallocates, tests, and runs our hash table. Has functions to
		retrieve individual names, student IDs, and hash codes of our
		UCSDStudent objects.

******************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif


class UCSDStudent : public Base {
	char * name;
	long studentnum;
public:
	UCSDStudent (char * nm, long sn) :
		name (strdup (nm)), studentnum (sn) {}


/*****************************************************************************
% Routine Name : ~UCSDStudent
% File :         Driver.c
%
% Description :  Frees memory used by UCSDStudent.
% 
% Parameters descriptions :
%
% name            description
% --------------- -----------------------------------------------------------
% <return>        Nothing is returned.
*****************************************************************************/
	~UCSDStudent (void) 
	{
		/* CALL DESTRUCTOR */
		free(name);
	}


/*****************************************************************************
% Routine Name : operator char * (void)
% File :         Driver.c
%
% Description :  Returns the name of the current UCSDStudent.
% 
% Parameters descriptions :
%
% name            description
% --------------- -----------------------------------------------------------
% <return>        A string representing the current object's name.
*****************************************************************************/
	operator char * (void) {
		return name;
	}


/*****************************************************************************
% Routine Name : operator long (void)
% File :         Driver.c
%
% Description :  Checks if the input parameter object is equal to the current
% 		 UCSDStudent.
% 
% Parameters descriptions :
%
% name            description
% --------------- -----------------------------------------------------------
% <return>        A boolean representing object equality is returned.
*****************************************************************************/
	operator long (void);
	long operator == (Base & base) {
		return ! strcmp (name, base);
	}


	ostream & Write (ostream & stream) {
		return stream << "name: " << name
			<< " Studentnum: " << studentnum;
	}

};


/*****************************************************************************
% Routine Name : operator long (void)
% File :         Driver.c
%
% Description :  Calculates a hashcode for the current UCSDStudent.
% 
% Parameters descriptions :
%
% name            description
% --------------- -----------------------------------------------------------
% <return>        An int representing the hashcode of the object is returned.
*****************************************************************************/
UCSDStudent :: operator long (void) {
	long sum = 0;

	for (long index = 0; name[index]; index++)
		sum += name[index];

	return sum;
}


int main (int argc, char * const * argv) {
	SymTab ST (5);
	char buffer[128];
	char command;
	long number;
	char option;

	/* initialize debug variable */
	HashTable::Set_Debug(0);

	 /* check command line options for debug display */
	 while ((option = getopt (argc, argv, "x")) != EOF) {

		 switch (option) {
			 case 'x': HashTable::Set_Debug(1);
				 break;
		 	}
	} 

	ST.Write (cout << "Initial Symbol Table:\n");
	while (cin) {
		command = NULL;	// reset command each time in loop
		cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite)"
		":  ";
		cin >> command;

		switch (command) {
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			if(!ST.Insert (new UCSDStudent (buffer, number))){

				ST.Write (cout << "\nFinal Symbol Table:\n");
				exit (0);
			}
			break;

		case 'l': {
			Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST.Lookup (& stu);
			
			if (found)
				found->Write (cout << "Student found!!!\n") 
				<< "\n";
			else
				cout << "Student " << buffer << " not there!\n";
			}
			break;

		case 'w':
			ST.Write (cout << "The Symbol Table contains:\n");
		}
	}
	ST.Write (cout << "\nFinal Symbol Table:\n");
}

