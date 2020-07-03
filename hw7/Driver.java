/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/18/2020
                                                        cs12wi20fi
                                Assignment Seven

File Name:      Driver.java
Description:    This program is used to define the UCSDStudent class, and test
                our Tree data structure.

******************************************************************************/

import java.io.*;

class UCSDStudent extends Base {

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
         * Checks if an object is equal to the current object.
	 *
	 * @param  An object that is to be checked for equality against a
	 *         UCSDStudent.
	 *
         * @return A boolean indicating if the 2 objects are equal
         */
	public boolean equals(Object object)
	{
		if (this == object)
		{
			return true;
		}

		if (!(object instanceof UCSDStudent))
		{
			return false;
		}

		UCSDStudent temp = (UCSDStudent) object;
		return this.name.equals(temp.getName());
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
		return (this.name.compareTo(base.getName()) > 0) ? true : false;
	}

        public String toString () {
                return "name:  " + name + "  studentnum:  " + studentnum;
        }
}

public class Driver {
        private static final short NULL = 0;

        public static void main (String [] args) {
        
                /* initialize debug states */
                Tree.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                Tree.debugOn();
                }


                /* The real start of the code */
                SymTab<UCSDStudent> symtab = 
                                new SymTab<UCSDStudent>("UCSDStudentTree");
                String buffer = null;
                char command;
                long number = 0;

                System.out.println ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = NULL; // reset command each time in loop
                        System.out.print ("Please enter a command:  " + 
                                "((a)llocate, is(e)mpty, (i)nsert, (l)ookup,"+
                                " (r)emove, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'a':
                                System.out.print
                                ("Please enter name of new Tree to " +
                                 "allocate:  ");
                                
                                buffer = MyLib.getline ();// formatted input
                                symtab = new SymTab<UCSDStudent>(buffer);
                                break;
			case 'e':
				if(symtab.isEmpty()){
					System.out.println("Tree is empty.");
				} else {
					System.out.println("Tree is "+
						"not empty.");
				}
				break;

                        case 'i':
                                System.out.print
                                ("Please enter UCSD student name to insert:  ");

                                buffer = MyLib.getline ();// formatted input

                                System.out.print
                                        ("Please enter UCSD student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create student and place in symbol table
                                symtab.insert(new UCSDStudent (buffer, number));
                                break;

                        case 'l': { 
                                UCSDStudent found;      // whether found or not

                                System.out.print
                                ("Please enter UCSD student name to lookup:  ");
                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println("Student found!");
                                        System.out.println(found);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;
                        
                        case 'r': { 
                                UCSDStudent removed; // data to be removed

                                System.out.print
                                ("Please enter UCSD student name to remove:  ");

                                buffer = MyLib.getline ();

                                UCSDStudent stu = new UCSDStudent (buffer, 0);

                                removed = symtab.remove(stu);

                                if (removed != null) {
                                        System.out.println("Student removed!"); 
                                        System.out.println(removed);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.println("The Symbol Table " +
                                "contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }
                System.out.println("\nFinal Symbol Table:\n" + symtab);
        }
}
