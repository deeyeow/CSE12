/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        3/16/2020
                                                        cs12wi20fi
                                Final Assessment

File Name:      Driver.java
Description:    This program is used to define the UCSDStudent class, and test
                our array-based Heap data structure.

******************************************************************************/

import java.io.*;
import java.util.*;	// required for Scanner object

/**
 * Represents a UCSDStudent object to be inserted in our Heap data structure.
 * Contains all methods that a UCSDStudent may require, such as comparing
 * greater elements.
 */
class UCSDStudent extends Base {

	// class data fields
        private String name;
        private long studentnum;

        /**
         * Constructor for UCSDStudent.
	 *
	 * @param  The name of the UCSDStudent.
	 *         The studentnum of the UCSDStudent.
	 *
         * @return Nothing
         */
	public UCSDStudent (String name, long studentnum)
	{
		this.name = new String(name);
		this.studentnum = studentnum;
	}

        /**
         * Getter method for the name of the UCSDStudent.
	 *
         * @return The name of the UCSDStudent
         */
	public String getName()
	{
		return this.name;
	}

        /**
         * Getter method for the studentnum of the UCSDStudent.
	 *
         * @return The studentnum of the UCSDStudent
         */
	public long getStudentNum()
	{
		return this.studentnum;
	}

        /**
         * Checks if an object is greater than the current object.
	 *
	 * @param  A Base that is to be compared to a UCSDStudent.
	 *
         * @return A boolean indicating if the Base is greater than the current
	 *         object
         */
	public boolean isGreaterThan (Base base)
	{
		return (this.name.compareTo(base.getName()) < 0) ? true : false;
	}

        public String toString () {
                return "name:  " + name + "  studentnum:  " + studentnum;
        }
}

/**
 * Driver to test our Heap data structure. Prompts user for commands, and
 * communicates with Heap.java accordingly.
 */
public class Driver {
        private static final short NULL = 0;

        public static void main (String [] args) {
        
                // initialize debug states
                Heap.debugOff();

                // check command line options
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                Heap.debugOn();
                }


                // The real start of the code
		System.out.print("Please enter the number of objects to be " +
		"able to store: ");

		// get user input
		int size = 0;
		boolean endProgramASAP = false;
		Scanner scanner = new Scanner(System.in);
		try{
			// get user input
			size = scanner.nextInt();
			scanner.nextLine();	// get rid of return
		}
		catch(Exception e)
		{
			endProgramASAP = true;
			System.out.println();
		}

		// only continue if no CTRL+D on first prompt
		if (!endProgramASAP)
		{
			// create new Heap of user-input size
                	Heap heap = new Heap(size);

			// initialize local variables used to capture user input
                	String buffer = null;
                	String command;
                	long number = 0;

                	System.out.println ("The Initial Heap:\n" + 
				heap.write());

                	while (true) {
				// reset command each time in loop
                        	command = null;
                        	System.out.print ("Please enter a command " + 
                                	"((i)nsert, (r)emove, (w)rite):  ");

                        	try {
					// get user input
                        		command = scanner.nextLine();

                        		switch (command) {
					// case for insert
                        		case "i":
                                		System.out.print
                                		("Please enter UCSD student " +
						"name to insert:  ");

						// formatted input
                                		buffer = scanner.nextLine();

                                		System.out.print
                                        	("Please enter UCSD " +
						"student number:  ");

						// get user input
                                		number = scanner.nextLong();
						scanner.nextLine();	// clear
									//return
				
						// check if heap is full
						if (heap.isFull())
						{
							System.out.println
							("Heap is full, could" +
							" not insert!!");
							break;
						}

                                		// create student and insert
                                		heap.insert(new UCSDStudent (
							buffer, number));
                                		break;
                        
					// case for remove
                        		case "r": 
						// check if heap is empty
						if (heap.isEmpty())
						{
							System.out.println
							("Heap is empty!");
							break;
						}

						// data to be removed
                                		UCSDStudent removed;

						// remove student from heap
                                		removed = (UCSDStudent)
							heap.remove();

                                		if (removed != null) {
                                        		System.out.println
							("Student removed!"); 
                                        		System.out.println
							(removed);
                                		}
                                		else
                                        		System.out.println 
							("student " + buffer + 
							" not there!");
						break;

					// case for write
                        		case "w":
						// write to console
                                		System.out.println
						("The Heap contains:");
						System.out.println
						(heap.write());
                        		}
                        	}
				// catch CTRL+D, end program
                        	catch (Exception e) {
                                	break;
                        	}
                	}

                	System.out.println("\nThe Final Heap contains:\n" + 
			heap.write());
		}
        }
}
