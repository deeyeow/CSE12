/****************************************************************************

                                                        Darren Yau
                                                        CSE 12, W20
                                                        1/14/2020
                                                        Your cs12x login
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/
#include <stdio.h>

#define COUNT ((long) (sizeof (long) << 1 )) /* number of hex digits to 
                                                display */
#define DECIMAL 10	/* to indicate base 10 */
#define HEX 16		/* to indicate base 16 */
#define ADD 12		/* To be used in adding values in do while loop in main */

/* Function prototypes */
void baseout (long, long, FILE *);
void decout (unsigned long, FILE *);
void hexout (unsigned long, FILE *);
void newline (FILE *);
long writeline (const char *, FILE *);

/* array used for ASCII conversion */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*--------------------------------------------------------------------------
Function Name:         List the function name.
Purpose:               Say why the function exists in a sentence or two.
Description:           Describe the underlying algorithm of how the function
                       is implemented in a few sentences.
Input:                 A list of the parameters with brief explanation of each.
Result:                A list of possible outcomes when the function is called.
                       Describe return value.
--------------------------------------------------------------------------*/
void baseout (long number, long base, FILE * stream) {
	/* YOUR CODE GOES HERE */
	int index = 0;
	int remainder = 0;
	char array[] = {0,0,0,0,0,0,0,0};

	do
	{
		remainder = number % base;
		array[index] = digits[remainder];
		number /= base;
		index++;
	} while (number != 0);

	while (index >= 0)
	{
		fputc(array[index], stream);
		index--;
	}
}


/*--------------------------------------------------------------------------
Function Name:         List the function name.
Purpose:               Say why the function exists in a sentence or two.
Description:           Describe the underlying algorithm of how the function
                       is implemented in a few sentences.
Input:                 A list of the parameters with brief explanation of each.
Result:                A list of possible outcomes when the function is called.
                       Describe return value.
--------------------------------------------------------------------------*/
void decout (unsigned long number, FILE * stream) {
	/* YOUR CODE GOES HERE */
	baseout (number, DECIMAL, stream);
}


/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderr
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout (unsigned long number, FILE * stream) {

	/* Output "0x" for hexidecimal. */
	writeline ("0x", stream);
	baseout (number, HEX, stream);
}


/*--------------------------------------------------------------------------
Function Name:         List the function name.
Purpose:               Say why the function exists in a sentence or two.
Description:           Describe the underlying algorithm of how the function
                       is implemented in a few sentences.
Input:                 A list of the parameters with brief explanation of each.
Result:                A list of possible outcomes when the function is called.
                       Describe return value.
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
	/* YOUR CODE GOES HERE */
	fputc('\n', stream);
}


/*--------------------------------------------------------------------------
Function Name:         List the function name.
Purpose:               Say why the function exists in a sentence or two.
Description:           Describe the underlying algorithm of how the function
                       is implemented in a few sentences.
Input:                 A list of the parameters with brief explanation of each.
Result:                A list of possible outcomes when the function is called.
                       Describe return value.
--------------------------------------------------------------------------*/
long writeline (const char * message, FILE * stream) {
	/* YOUR CODE GOES HERE */
	int index = 0;
	while (message[index] != '\0')
	{
		fputc(message[index], stream);
		index++;
	}
	return 0;
}


int main (int argc, char *const* argv) {

	int element = 0;
	int count = 0;

	do {
		element += ADD;
		count++;

	}while(count < 3);

	writeline ("def", stdout);
	writeline ("abc", stderr);
	newline(stderr);
	newline(stdout);
	decout (123, stdout);
	newline(stdout);
	decout (0, stdout);
	newline(stdout);
	hexout (0xFEEDDAD, stdout);
	newline(stdout);
	return 0;
}
