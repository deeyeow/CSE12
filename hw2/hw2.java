/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
        private Character character;

        public MyLibCharacter (int ch) {
                character = new Character ( (char) ch );
        }

        public char charValue () {
                return character.charValue ();
        }

        public String toString () {
                return "" + character;
        }
}

public class hw2 {
	private static final int ASCII_ZERO = 48;

	private static final int CR = 13;		// Carriage Return
	private static final int MAXLENGTH = 80;	// Max string length

	private static final int EOF = -1;		// process End Of File

	private static final long COUNT = 16;		// # of hex digits

	private static final long DECIMAL = 10;		// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// for ASCII conversion
	     new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

	private static final String DEBUG_GETALINE = 
		"[*DEBUG:  The length of the string just entered is ";

	private static final String DIGIT_STRING = "digit ";
	private static final String REENTER_NUMBER ="\nPlease reenter number: ";
	private static final String OUT_OF_RANGE = " out of range!!!\n";
	private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
	private static final String DEBUG_WRITELINE =
		"\n[*DEBUG:  The length of the string displayed is ";

	private static Stack<MyLibCharacter> InStream =
		new Stack<MyLibCharacter>();

	private static boolean debug_on = false;
	private static long hexCounter = 0; // counter for the number hex digits

	/*---------------------------------------------------------------------
		Copy your function header and code for baseout from hw1
	---------------------------------------------------------------------*/
	
	/**
	* Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
	private static void baseout (long number, long base, PrintStream stream) {
		
		char[] arr = new char[(int)COUNT];	//initialize new array, with max size of COUNT
		
		int index = 0;	//initialize curr index to 0
		int remainder = 0;	//initialize remainder to 0
		
		//loop until quotient cannot be operated on anymore
		do
		{
			remainder = (int)number % (int)base;	//mod quotient by base to get remainder
			arr[index] = digits[remainder];	//store remainder in array at index
			number /= base;	//divide quotient by base to get new quotient 
			index++;	//increment index to get next location in array 			
		} while (number != 0);

		//loop until array is completely traversed
		while (index >= 0)
		{
			fputc(arr[index], stream);	//put the digit from array into stream
			index--;	//decrement index to traverse array correctly (backwards)
		}
	}

	/**
	 * Clears the buffer
	 *
	 * @param   char to be flushed
	 */ 
        public static void clrbuf (int character) {
		
		//while not end of char
		while (character != '\n')		
		{
			character = fgetc(System.in);	//continuously gets(flushes) the next char
		}
        }


	/** 
	 * Reads in a decimal number from stdin and converts it from a sequence of ASCII characters into a decimal integer.
	 *
	 * @return Converted decimal integer.
	 */
        public static long decin() {
		
		int sum = 0;	//initialize sum to keep track of final answer
		int temp = 0;	//initialize temp to check for overflows
		int digit = 0;	//initialize digit to keep track of next digit to add
		
		int input = fgetc(System.in);	//get next char

		//iterate over every char in line
		while (input != '\n')
		{
			//check for EOF
			if (input == EOF)
			{
				return EOF;
			}
			
			//check if input is in range (1-9)
			if (!(input >= '0' && input <= '9'))
			{
				digiterror(input, OUT_OF_RANGE);	//throw digiterror with OUT_OF_RANGE
				sum = (int)decin();	//call another instance of decin(), and set sum to return value
				break;	//skip while loop, already processed in other instance
			}
			
			temp = sum;	//keep track of old sum
			sum *= 10;	//multiply by 10 to move by 1 digits place
			digit = input - '0';	//get actual int representation of char
			sum += digit;	//add digit to sum
			
			//check if new and old sum are the same (overflow)
			if ((sum - digit) / 10 != temp)
			{
				digiterror(input, CAUSED_OVERFLOW);	//throw digiterror with CAUSED_OVERFLOW
				sum = (int)decin();	//call another instance of decin(), and set sum to return value
				break;	//skip while loop, already processed in other instance
			}
			
			input = fgetc(System.in);	//get next char
		}	

		return sum;
        }


	/**
        * Takes in a positive number and displays it in decimal.
        *
        * @param    positive numeric value to be displayed.
        * @param    Where to display, likely System.out or System.err.
        */
	public static void decout (long number, PrintStream stream) {
		baseout (number, DECIMAL, stream);	//call baseout with DECIMAL parameter
	}



	/*---------------------------------------------------------------------
	Function Name:          digiterror
	Purpose:                This function handles erroneous user input.
	Description:            This function  displays and error message to the
				user, and asks for fresh input.
	Input:                  character: The character that began the problem.
				message:  The message to display to the user.
	Result:                 The message is displayed to the user.
				The result in progress needs to be set to 0 in
				decin after the call to digiterror.
	----------------------------------------------------------------------*/
	public static void digiterror (int character, String message) {

		/* handle error */
		clrbuf (character);

		/* output error message */
		writeline (DIGIT_STRING, System.err);
		fputc ( (char)character, System.err);
		writeline (message, System.err);

		writeline (REENTER_NUMBER, System.err);
	}


	/**
	 * Reads characters from standard input and stores them into the message
	 * array.
	 *
	 * @param   Array to store received input.
	 * @param   The maximum number of characters allowed in the string.
	 * @return <code>long</code> Length of the string.
	 */
        public static long getaline( char message[], int maxlength ) {
		
		int input = fgetc(System.in);	//get first char in buffer
		int index = 0;	//initialize index to 0
		maxlength = MAXLENGTH;	
		//loop until reach maxlength, or input is EOL
		do 
		{
			if (input == EOF)	//check if end of file
			{
				return EOF;
			}
			message[index] = (char)input;	//set message at index to user input
			index++;	//increment index
			input = fgetc(System.in);	//get next char in buffer
			
		} while (index < maxlength - 1 && input != '\n');

		clrbuf(input);	//clear buffer
		message[index] = '\0';	//add null char to end of message array
		
		if (debug_on)
		{
			System.err.println(DEBUG_GETALINE + index);	//print debug statement
		}

		return index;	//return length of message array (string)
        }


        /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void hexout (long number, PrintStream stream) {

                // Output "0x" for hexidecimal.
                writeline ("0x", stream);
                baseout (number, HEX, stream);
        }


        /**
        * Returns a character from the input stream.
        *
        * @return  <code>char</code> 
        */
        public static int fgetc(InputStream stream) {

                char ToRet = '\0';

                // Check our local input stream first.
                //   If it's empty read from System.in
                if (InStream.isEmpty ()) {

                        try {
                                // Java likes giving the user the
                                // CR character too. Dumb, so just 
                                // ignore it and read the next character
                                // which should be the '\n'.                  
                                ToRet = (char) stream.read ();
                                if (ToRet == CR)
                                        ToRet = (char) stream.read ();
                                
                                // check for EOF
                                if ((int) ToRet == 0xFFFF)
                                        return EOF;
                        }

                        // Catch any errors in IO.
                        catch (EOFException eof) {

                                // Throw EOF back to caller to handle
                                return EOF;
                        }

                        catch (IOException ioe) {

                                writeline ("Unexpected IO Exception caught!\n",
                                                        System.out);
                                writeline (ioe.toString (), System.out);
                        }

                }

                // Else just pop it from the InStream.
                else
                        ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
                return ToRet;
        }


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
        }


	/**
        * Prints out a newline character.
        * @param    Where to display, likely System.out or System.err.
        *
        */
        public static void newline ( PrintStream stream ) {
		fputc('\n', stream);	//put a newline into stream
        }

	 /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @param    Where to display, likely System.out or System.err.
        * @return     <code>int</code> The length of the string.
        */
        public static long writeline (String message, PrintStream stream) {
		int index = 0;
		
		//iterate over every char in array
		while (index < message.length())
		{
			fputc(message.charAt(index), stream);	//write char into buffer
			index++;
		}

		//print debug message if debug mode turned on
		if (debug_on)
		{
			System.err.println(DEBUG_WRITELINE + index);
		}

		return index;
        }
	
	/**
	* Places back a character into the input stream buffer.
	*
	* @param    A character to putback into the input buffer stream.
	*/
	public static void ungetc (int ToPutBack) {

		// Push the char back on our local input stream buffer.
		InStream.push (new MyLibCharacter (ToPutBack));
	}


	public static void main( String[] args ) {

		char buffer[] = new char[MAXLENGTH];       /* to hold string */

		long number;                  /* to hold number entered */
		long strlen;                  /* length of string */
		long base;		      /* to hold base entered */

		/* initialize debug states */
		debug_on = false;

		/* check command line options for debug display */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				debug_on = true;
		} 

		/* infinite loop until user enters ^D */
		while (true) {
			writeline ("\nPlease enter a string:  ", System.out);

			strlen = getaline (buffer, MAXLENGTH);
			newline (System.out);

			/* check for end of input */
			if ( EOF == strlen )
				break;

			writeline ("The string is:  ", System.out);
			writeline ( new String(buffer), System.out);

			writeline ("\nIts length is ", System.out);
			decout (strlen, System.out);
			newline (System.out);

			writeline ("\nPlease enter a decimal number:  ", 
			             System.out);
			if ((number = decin ()) == EOF)
				break;

			writeline ("\nPlease enter a decimal base:  ", 
			             System.out);
			if ((base = decin ()) == EOF)
				break;

			/* correct bases that are out of range */
			if (base < 2)
				base = 2;
			else if (base > 36)
				base = 36;

			newline (System.out);

			writeline ("Number entered in base ", System.out);
			decout (base, System.out);
			writeline (" is: ", System.out);
			baseout (number, base, System.out);

			writeline ("\nAnd in decimal is:  ", System.out);
			decout (number, System.out);

			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number, System.out);

			writeline ("\nNumber entered multiplied by 8 is:  ", 
			           System.out);
			decout (number << 3, System.out);
			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number << 3, System.out);

			newline (System.out);
		}
	}
}
