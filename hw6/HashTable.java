/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/9/2020
                                                        cs12wi20fi
                                Assignment Six

File Name:      HashTable.java
Description:    This program allows variable assignment to the calculator using
		a Symbol Table, that we will implement with a custom hash table.
		Our hash table will be double hashing using probe count array to
		track insertion attempts. This program will also use the
		FairShare algorithm to resolve any collisions.

******************************************************************************/
public class HashTable extends Base {

	private static int counter = 0;	// number of HashTables so far
        private static boolean debug;        // allocation of debug state

        // set in locate, last location checked in hash table 
        private int index = 0; 

        // set in insert/lookup, count of location in probe sequence
        private int count = 0;
        
        private int probeCount[];   // where we are in the probe sequence 
        private long occupancy;     // how many elements are in the Hash Table
        private int size;           // size of Hash Table
        private Base table[];       // the Hash Table itself ==> array of Base
        private int tableCount;     // which hash table it is

        // messages
        private static final String AND = " and ";
        private static final String DEBUG_ALLOCATE = " - Allocated]\n";
        private static final String DEBUG_LOCATE = " - Locate]\n";
        private static final String DEBUG_LOOKUP = " - Lookup]\n";
        private static final String BUMP = "[Bumping To Next Location...]\n";
        private static final String COMPARE = " - Comparing ";
        private static final String FOUND_SPOT = " - Found Empty Spot]\n";
        private static final String HASH = "[Hash Table ";
	private static final String HASH_VAL = "[Hash Value Is ";
        private static final String INSERT = " - Inserting ";
        private static final String PROCESSING = "[Processing ";
        private static final String TRYING = "[Trying Index ";

        
/*-----------------------------------------------------------------------------
Function Name:		debugOn
Purpose:		Turns on debugging for this list.
Description:		Sets debug to TRUE.
Input:			Nothing.
Result:			Debug is set. Nothing is returned.
-----------------------------------------------------------------------------*/
        public static void debugOn () {
                debug = true;
        }

/*-----------------------------------------------------------------------------
Function Name:		debugOff
Purpose:		Turns off debugging for this list.
Description:		Sets debug to FALSE.
Input:			Nothing.
Result:			Debug is set. Nothing is returned.
-----------------------------------------------------------------------------*/
        public static void debugOff () {
                debug = false;
        }

/*-----------------------------------------------------------------------------
Function Name:		HashTable
Purpose:		Allocates and initializes the memory associated with a
			hash table.
Description:		Creates a hash table and the corresponding probe table
			using the "new" Java operator.
Input:			int sz: The size of the hash table to be created. Must
			be prime!
Result:			A pointer to the hash table.
-----------------------------------------------------------------------------*/
        public HashTable (int sz) {
		
		//set corresponding variables to input parameters
		occupancy = 0;
		size = sz;

		//create hash table, and corresponding probe table
		table = new Base[size];
		probeCount = new int[size];

		//assign tableCount and increment counter
		tableCount = ++counter;

		//print debug msg
		if (debug)
		{
			System.err.println(HASH + tableCount + DEBUG_ALLOCATE);
		}
        }


        /**
         * This method will insert the element in the hash table.
         * If the element cannot be inserted, false will be returned.
         * If the element can be inserted, the element is inserted
         * and true is returned.  Duplicate insertions will
         * cause the existing element to be deleted, and the duplicate
         * element to take its place.
         *
         * @param   element       The element to insert.
         * @param   initialCount  Where to start in probe seq (recursive calls) 
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Base element, int initialCount) {
               
		//set initial count
		count = initialCount;

		Base tempElement;
		int tempCount;

		//check if already probed entire hash table for empty or valid
		//space
		if (count >= size)
		{
			return false;
		}

		//if element not located, which means can insert
		if (!locate(element))
		{
			//index is empty, insert now
			if (table[index] == null)
			{
				if (debug)
				{
					System.err.println(HASH + tableCount +
					INSERT + element.getName() + "]");
				}

				table[index] = element;
				probeCount[index] = count;

				//item is added, so increment occupancy
				occupancy++;
			}

			//index is occupied by another element, recurse
			else
			{
				if (debug)
				{
					System.err.println(BUMP);
				}

				//store location values
				tempElement = table[index];
				tempCount = probeCount[index];

				//put element into location
				table[index] = element;
				probeCount[index] = count;

				//recurse with element in previous location
				insert(tempElement, ++tempCount);
			}
		}

		//element located, which means there is duplicate
		else
		{
			if (debug)
			{
				System.err.println(HASH + tableCount + INSERT +
				element.getName() + "]");
			}

			//put element into location
			table[index] = element;
			probeCount[index] = count;

			//duplicate is replaced, so no need to increment
			//occupancy
		}

		return true;
        }


        /**
         * This method will locate the location in the
         * table for the insert or lookup.
         *
         * @param   element  The element needing a location.
         * @return  true if item found, or false if not found
         */
        private boolean locate (Base element) {
                
		//set hashcode, initial hashing location, and increment
		int hashcode = element.hashCode();
		int initial = hashcode % size;
		int increment = hashcode  % (size - 1) + 1;

		//use count to quickly locate index, instead of starting at the
		//beginning
		index = (initial + (count - 1) * increment) % size;

		//print debug msg
		if (debug)
		{
			System.err.println(HASH + tableCount + DEBUG_LOCATE);
			System.err.println(PROCESSING + element.getName() + 
				"]");
			System.err.println(HASH_VAL + hashcode + "]");
		}

		//while hash table is not full
		while (occupancy < size)
		{
			if (debug)
			{
				System.err.println(TRYING + index + "]");
			}

			//check if index is empty, which means not found
			if (table[index] == null)
			{
				if (debug)
				{
					System.err.println(HASH + tableCount +
					FOUND_SPOT);
				}

				return false;
			}

			//if not null, occupied by another element
			else 
			{
				if (debug)
				{
					System.err.println(HASH + 
					tableCount + COMPARE + 
					table[index].getName() + AND + 
					element.getName() + "]");
				}

				//check if element is duplicate
				if (table[index].equals(element))
				{
					return true;
				}
				
				//if not duplicate, location occupied by
				//different element
				else
				{
					//found index to replace using FairShare
					//algorithm, which means element not
					//found
					if (probeCount[index] < count)
					{
						return false;
					}
				}
			}

			//increment number of locations checked
			count++;

			//compute next index to check
			index = (index + increment) % size;
		}

		//hash table is full, can't perform any more operations
		return true;
        }


        /**
         * This method will lookup the element in the hash table.  If
         * found a pointer to the element is returned.  If the element
         * is not found, NULL will be returned to the user.
         *
         * @param   element  The element to look up.
         * @return  A pointer to the element if found, else NULL
         */
        public Base lookup (Base element) {
                
		//setting count
		count = 1;

		if (debug)
		{
			System.err.println(HASH + tableCount + DEBUG_LOOKUP);
		}

		//if locate returns true, element has been found
		if (locate(element))
		{
			return table[index];
		}

		//else, element not found and return null
		return null;
        }


        /**
         * Creates a string representation of the hash table. The method 
         * traverses the entire table, adding elements one by one ordered
         * according to their index in the table. 
         *
         * @return  String representation of hash table
         */
        public String toString () {
                String string = "Hash Table " + tableCount + ":\n";
                string += "size is " + size + " elements, "; 
                string += "occupancy is " + occupancy + " elements.\n";

                /* go through all table elements */
                for (int index = 0; index < size; index++) {

                        if (table[index] != null) {
                                string += "at index " + index + ": ";
                                string += "" + table[index];
                                string += " with probeCount: "; 
                                string += probeCount[index] + "\n";
                        }
                }

                return string;
        }
}
