/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

public class hw1 {
	private static final long COUNT = 16;	// number of hex digits to display
	private static final long DECIMAL = 10;	// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16
	private static final int ADD = 12;	// to add to the var in the main loop

	private static final char digits[] = 	// used for ASCII conversion
		new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
	private static long hexCounter = 0; // counter for the number hex digits


	/**
	* Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
	private static void baseout (long number, long base, PrintStream stream) {
		// YOUR CODE GOES HERE
		char[] arr = new char[(int)COUNT];

		int index = 0;
		int remainder = 0;
		do
		{
			remainder = (int)number % (int)base;
			arr[index] = digits[remainder];
			number /= base;
			index++;
		} while (number != 0);

		while (index >= 0)
		{
			fputc(arr[index], stream);
			index--;
		}
	}


        /**
        * Takes in a positive number and displays it in decimal.
        *
        * @param    positive numeric value to be displayed.
        * @param    Where to display, likely System.out or System.err.
        */
	public static void decout (long number, PrintStream stream) {
		// YOUR CODE GOES HERE
		baseout (number, DECIMAL, stream);
	}


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
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
        * Prints out a newline character.
        * @param    Where to display, likely System.out or System.err.
        *
        */
        public static void newline ( PrintStream stream ) {
		// YOUR CODE GOES HERE
		fputc('\n', stream);
        }


        /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @param    Where to display, likely System.out or System.err.
        * @return     <code>int</code> The length of the string.
        */
        public static long writeline (String message, PrintStream stream) {
                // YOUR CODE GOES HERE
		for (int i = 0; i < message.length(); i++)
		{
			fputc(message.charAt(i), stream);
		}
		return 0;
        }


	public static void main( String[] args ) {

		int element = 0;
		int count = 0;
		while (count < 3){
		
			element += ADD;
			count++;
		}
		writeline ("abc", System.err);
		newline(System.out);
		writeline ("def", System.out);
		newline(System.out);
		decout (123, System.out);
		newline(System.out);
		decout (0, System.out);
		newline(System.out);
		hexout (0xFEEDDAD, System.out);
		newline(System.out);
	}
}
