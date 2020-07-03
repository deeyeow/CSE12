/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/9/2020
                                                        cs12wi20fi
                                Assignment Six

File Name:      Driver.java
Description:    The driver file for testing our hash table. Allocates,
		deallocates, tests, and runs our hash table. Has functions to
		retrieve individual names, student IDs, and hash codes of our
		UCSDStudent objects.

******************************************************************************/
import java.io.EOFException;

class UCSDStudent extends Base {
        private String name;
        private long Studentnum;

        public String toString () {

                return "name: " + name + " Studentnum: " + Studentnum;
        }

        
/*-----------------------------------------------------------------------------
Function Name:		UCSDStudent
Purpose:		Parameterized constructor.
Description:		Creates a UCSDStudent object with the specified
			parameters.
Input:			String name: The name of the student.
			long StudentNum: The ID of the student.
Result:			Nothing is returned.
-----------------------------------------------------------------------------*/
	public UCSDStudent (String name, long Studentnum)
	{
		this.name = name;
		this.Studentnum = Studentnum;
	}

/*-----------------------------------------------------------------------------
Function Name:		UCSDStudent
Purpose:		Parameterized constructor.
Description:		Creates a UCSDStudent object with the specified
			parameters.
Input:			UCSDStudent student: A UCSDStudent object to be copied.
Result:			Nothing is returned.
-----------------------------------------------------------------------------*/
	public UCSDStudent (UCSDStudent student)
	{
		name = student.name;
		Studentnum = student.Studentnum;
	}

/*-----------------------------------------------------------------------------
Function Name:		getName
Purpose:		Gets the name of a UCSDStudent.
Description:		Returns the name of the current UCSDStudent.
Input:			Nothing.
Result:			A string representing the current object's name is
			returned.
-----------------------------------------------------------------------------*/
	public String getName()
	{
		return name;
	}

/*-----------------------------------------------------------------------------
Function Name:		equals
Purpose:		Checks if the input parameter object is equal to the
			current UCSDStudent.
Description:		Checks if they are the same object, then checks if they
			are instances of each other, finally compares their
			contents.
Input:			Object other: The object to be compared.
Result:			A boolean representing object equality is returned.
-----------------------------------------------------------------------------*/
	public boolean equals (Object other)
	{
		if (this == other)
		{
			return true;
		}
		
		if (!(other instanceof UCSDStudent))
		{
			return false;
		}

		UCSDStudent student = (UCSDStudent)other;
		return name.equals (student.getName());
	}

/*-----------------------------------------------------------------------------
Function Name:		hashCode
Purpose:		Calculates a hashcode for the current UCSDStudent.
Description:		Loops through the name of the current object, and adds
			the ASCII value of each char to get the hashcode.
Input:			Nothing.
Result:			An int representing the hashcode of the object is
			returned.
-----------------------------------------------------------------------------*/
	public int hashCode()
	{
		int retval = 0;
		int index = 0;

		while (index != name.length())
		{
			retval += name.charAt(index);
			index++;
		}

		return retval;
	}

/*-----------------------------------------------------------------------------
Function Name:		assign
Purpose:		Assigns a student ID to the current UCSDStudent.
Description:		Sets the Studentnum of the current object to the input
			parameter, then returns a pointer to the object.
Input:			long Studentnum: The student ID to set the current
			object to.
Result:			The current UCSDStudent object is returned.
-----------------------------------------------------------------------------*/
	public UCSDStudent assign (long Studentnum)
	{
		UCSDStudent retval;

		this.Studentnum = Studentnum;
		retval = new UCSDStudent(this);

		return retval;
	}
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
