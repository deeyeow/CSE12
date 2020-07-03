/****************************************************************************

                                                        Darren Yau
                                                        CSE 12, W20
                                                        1/27/2020
                                                        cs12wi20fi
                                Assignment Four

File Name:      calc.c
Description:    An program that simulates a top-of-the-line calculate that
		performs advanced commands such as addition, subtraction,
		multiplication, division, exponentiation, and factorial! The
		program will accept mathematical expressions in "infix" notation
		and convert it to its "postfix" equivalent.
****************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE 8

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*-----------------------------------------------------------------------------
Function Name:		eval
Purpose:		This function evaluates mathematical expressions from
			"postfix" notation. 
Description:		First reverses all elements in stack1 onto stack2 to get
			the correct ordering. Then pushes numbers onto stack1.
			When encounter a operator, pop 2 numbers from stack1 and
			perform the correct operation given the popped operator.
			Lastly pushes the result back onto stack1.
Input:			Stack * stack1: A pointer to stack1 containing the
			mathematical expression in "postfix" notation.
Result:			Returns a long that is the result of the mathematical
			expression.
-----------------------------------------------------------------------------*/
long eval (Stack * stack1) {

     Stack * stack2 = new_Stack(CALCSTACKSIZE);	/*initialize stack2 */

     /* declare popped, operand1, operand2, index, and result variables */
     long popped;
     long operand1;
     long operand2;
     long index;
     long result;

     /* reverse stack1 onto stack2 */ 
     while (!isempty_Stack(stack1))
     {
     	pop(stack1, &popped);
	push(stack2, popped);
     }

     /* operate on stack2 until empty*/
     while (!isempty_Stack(stack2))
     {
     	/* pop next element from stack2 */
     	pop(stack2, &popped);
	
	/* if popped element is digit, push onto stack1 (operators are neg)*/
	if (popped >= 0)
	{
		push(stack1, popped);
	}

	/* pop 2 operands from stack1, then extract index of operator from 
	masked bit. Perform operation using operations in function array, then 
	push resulting answer to stack1 */
	else
	{
		pop(stack1, &operand1);
		pop(stack1, &operand2);
		
		index = INDEX(popped);
		
		result = functions[index](operand1, operand2);	/* perform
		operation by indexing functions array and passing in operands */
		push(stack1, result);
	}
     }
     delete_Stack(&stack2);	/* don't forget to deallocate stack2 */
     pop(stack1, &result);	/* what's left on stack1 will be the result */
     return result;
}


/*-----------------------------------------------------------------------------
Function Name:		intopost
Purpose:		This function converts an "infix" mathematical
			expression into its "postfix" equivalent. 
Description:		First process each character of the input in turn. If
			character is a digit, continue to read digits until you
			read a non-digit. Then push this number on stack1. If
			character is '(', push it onto stack2. If character is
			')', pop stack2 and push onto stack1 until encounter
			first '('. If character is operator, pop stack2 and push
			onto stack1 until stack2 is empty or popped has higher
			priority than character. Then push character onto
			stack2.
Input:			Stack * stack1: A pointer to empty stack1.
Result:			Returns a long that indicates whether the function
			executed successfully.
-----------------------------------------------------------------------------*/
long intopost (Stack * stack1) {
     
     Stack * stack2 = new_Stack(CALCSTACKSIZE);	/* initialize stack2 */

     /* declare digit, popped, and peeked variables */
     long digit = 0;
     long popped;
     long peeked;

     int c = fgetc(stdin);	/* get next character of user input */

     /* iterate until end of user input */
     while (c != '\n')
     {
     	/* if encounter EOF, stop program */
     	if (c == EOF)
	{
		delete_Stack(&stack2);	/* don't forget to deallocate stack2 */
		return EOF;
	}

	/* if encounter blank space, continue */
	if (c == ' ')
	{
		c = fgetc(stdin);	/* get next char in stdin stream */
		continue;
	}

	/* if encounter digit, call decin to parse digits, and push to stack1 */
	if (isdigit(c))
	{
		ungetc(c, stdin);	/* put back digit that you took from
		stream, so decin works properly */
		digit = decin();
		push(stack1, digit);
		c = fgetc(stdin);
	}

	/* if encounter open parenthesis, push onto stack2 (to act as a sentinel
	 * in stack2) */
	else if (c == '(')
	{
		push(stack2, c);
		c = fgetc(stdin);
	}

	/* if encounter closed parenthesis, pop all elements from stack2 and
	 * push onto stack1, until meet the corresponding open parenthesis on
	 * stack2 */
	else if (c == ')')
	{
		pop(stack2, &popped);

		while (popped != '(')	/* keep popping until encounter an open
		parenthesis */
		{
			push(stack1, popped);	/* push popped element onto
			stack1 */
			pop(stack2, &popped);	/* pop next element from stack2 
			*/
		}
		c = fgetc(stdin);	/* get next char in stdin stream */
	}

	/* if encounter operator, pop all elements from stack2 and push onto
	 * stack1, until meet an operator with lower priority on stack2. Then
	 * push the operator onto stack2 */
	else
	{
		if (!isempty_Stack(stack2))	/* only peek element if stack2 
		still has elements */
		{
			top(stack2, &peeked);
		}
		while (PRIORITY(peeked) >= PRIORITY(setupword(c)))	/* only 
		loop until meet operator with lower priority on stack2*/
		{
			if (isempty_Stack(stack2))	/* stop looping if
			stack2 is empty */
			{
				break;
			}
			pop(stack2, &popped);
			push(stack1, popped);

			/* peek next element if not empty */
			if (!isempty_Stack(stack2))
			{
				top(stack2, &peeked);
			}
		}
		push(stack2, setupword(c));	/* push char onto stack2 */
		c = fgetc(stdin);	/* get next char in stdin stream */
	    }
	}

	/* after processing all char, pop anything remaining on stack2,
	 * pushing all elements to stack1 */
	while (!isempty_Stack(stack2))
	{
		pop(stack2, &popped);
		push(stack1, popped);
	}
	delete_Stack(&stack2); /* don't forget to deallocate stack2 */
	return 1;
}

static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}


/*-----------------------------------------------------------------------------
Function Name:		exponent
Purpose:		This function performs exponentiation given a base and a
			power.
Description:		Recursively multiply base with base, decrementing power
			by 1 each time. End recursion when power is 1.
Input:			long power: A long that represents the power to
			exponentiate to.
			long base: A long that represents the base to
			exponentiate on.
Result:			Returns a long that is the result of the mathematical
			expression.
-----------------------------------------------------------------------------*/
static long exponent (long power, long base) {
     /* do recursion, decrementing power by 1 each iteration */
     if (power == 1)
     {
     	return base;	/* base case, anything to the power of 1 is itself */
     }
     return base * exponent(power - 1, base);
}

/*-----------------------------------------------------------------------------
Function Name:		fact
Purpose:		This function performs factorial given a number.
Description:		Recursively multiply xxx with (xxx - 1), decrementing 
			xxx by 1 each time. End recursion when xxx is 1.
Input:			long xxx: A long that represents the number to perform
			factorial on.
			long ignored: A long that is to be ignored.
Result:			Returns a long that is the result of the mathematical
			expression.
-----------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
     /* do recursion, decrementing xxx by 1 each iteration */
     if (xxx == 1)
     {
     	return xxx;	/* base case, anything times 1 is itself */
     }
     return xxx * fact(xxx - 1, ignored);
}

static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*-----------------------------------------------------------------------------
Function Name:		setupword
Purpose:		Constructor function for longs representing operators to
			be stored on the stacks. The representation for the
			operators will have everything associated with that
			operator: a distinction from numbers, the index in the
			functions array corresponding to that operator, and the
			priority of the operator and the ASCII code.
Description:		First find the index in the functions array
			corresponding to that operator. Then apply bit masking
			and shifting to encode the SIGN_BIT, index, and ASCII
			code into a long.
Input:			int character: An int that represents the ASCII value of
			the operator.
Result:			Returns a long that encodes the SIGN_BIT, index, and
			ASCII code of the original character.
-----------------------------------------------------------------------------*/
static long setupword (int character) {
     /* initialize index and arraySize variables */
     int index = 0;
     int arraySize = sizeof(operators) / sizeof(char);

     /* iterate through operators array until found operator, index will be
      * location of that operator */
     for (; index < arraySize; index++)
     {
     	if (operators[index] == character)
	{
		break;
	}
     }
     return SIGN_BIT | (index << BYTE) | character;	/* OR SIGN_BIT to
     indicate operator, OR index shifted by 8 to store index of
     corresponding operator in correct position, OR character to last 4
     positions to store ASCII value */
}

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

