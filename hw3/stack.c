/****************************************************************************

                                                        Darren Yau
                                                        CSE 12, W20
                                                        1/21/2020
                                                        cs12wi20fi
                                Assignment Three

File Name:      stack.c
Description:    An array-based stack of longs. The user can create, delete, 
		and modify stacks through push, pop, and empty functions. The
		user can also check if a stack is empty or full, as well as 
		peek at the top element and get the current capacity.
****************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}

/*-----------------------------------------------------------------------------
Function Name:		delete_Stack
Purpose:		This destructor function deletes a stack.
Description:		First deallocates all memory associated with the stack
			and sets its pointer in the calling function to NULL. 
			Then decrements the stack counter.
Input:			Stack **spp: A pointer to a pointer to the stack.
Result:			Returns a long indicating success or failure of stack
			deletion.
-----------------------------------------------------------------------------*/
long delete_Stack (Stack ** spp) {
    /* check if stack pointer or stack is null */
    if (!spp || !*spp || stack_counter == 0)
    {
	fprintf(stderr, DELETE_NONEXIST);	/* nonexist stack err */
	return 0;
    }
    free(*spp - STACK_OFFSET);	/* free spp memory, minus stack 
    offset since beginning of entire stack is *spp + STACK_OFFSET */
    *spp = NULL;	/* now can set stack pointer to null */
    stack_counter--;	/* decrement stack counter */
    /* if user wants debug msgs */
    if (debug)
    {
    	fprintf(stderr, DEALLOCATE, (long)(stack_counter + 1));	/* print msg */
    }
    return 1;

}

/*-----------------------------------------------------------------------------
Function Name:		empty_Stack
Purpose:		This function empties all the contents of a stack.
Description:		Moves the next available pointer back to the beginning
			of the stack, effectively overwriting all content.
Input:			Stack *this_Stack: A pointer to the current stack.
Result:			Returns nothing.
-----------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
    /* check if stack is null */
    if (stack_counter == 0)
    {
    	fprintf(stderr, EMPTY_NONEXIST);	/* nonexist stack err */
    }
    else
    {
    	this_Stack[STACK_POINTER_INDEX] = 0;	/* move pointer to beginning
	of stack, which will overwrite all previous data when adding */
    }
}
        
/*-----------------------------------------------------------------------------
Function Name:		isempty_Stack
Purpose:		Checks if the current stack is empty or not.
Description:		Checks if the next available pointer is pointing to the
			beginning. If yes, no elements have been written yet,
			and the stack is empty. If not, the stack is not empty.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns TRUE if the stack is empty, FALSE if the stack
			is not empty.
-----------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
    /* check if a stack even exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, ISEMPTY_NONEXIST);	/* nonexist stack err */
	return 0;
    }
    /* check if stack_pointer_index is pointing to beginning */
    if (this_Stack[STACK_POINTER_INDEX] == 0)
    {
    	return 1;
    }
    /* if not pointing to beginning, stack has elements and not empty! */
    else
    {
    	return 0;
    }
}

/*-----------------------------------------------------------------------------
Function Name:		isfull_Stack
Purpose:		Checks if the current stack is full or not.
Description:		Checks if the next available pointer is pointing to the
			end. If yes, all available spaces have been written,
			and the stack is full. If not, the stack is not full.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns TRUE if the stack is full, FALSE if the stack
			is not empty.
-----------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
    /* check if a stack even exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, ISFULL_NONEXIST);	/* nonexist stack err */
	return 1;
    }
    /* check if stack_pointer_index is pointing to end */
    if (this_Stack[STACK_POINTER_INDEX] == this_Stack[STACK_SIZE_INDEX])
    {
    	return 1;
    }
    /* if not pointing to end, stack isn't completely full */
    else
    {
    	return 0;
    }
}

/*-----------------------------------------------------------------------------
Function Name:		new_Stack
Purpose:		This constructor function allocates and initializes a
			new Stack object. It allocates memory to hold stacksize
			number of longs, initializes the stack infrastructure,
			and returns a pointer to the first storage space in
			the stack.
Description:		Allocates memory for a new stack, given user-defined
			stacksize. Then creates a pointer to point to the new
			memory, and initializes a stack with the correct
			properties. Lastly, increments the stack counter.
Input:			unsigned long stacksize: The size of the stack to be
			created.
Result:			Returns a pointer to the memory address of the new 
			stack, plus the offset.
-----------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
    /* allocate (stacksize + offset) * longsize bytes of memory */ 
    void * memory = malloc((STACK_OFFSET + stacksize) * sizeof(long));
    /* set stack pointer to beginning of memory + offset */
    Stack * this_Stack = (Stack *)memory + STACK_OFFSET;
    /* set stack_pointer_index to beginning */
    this_Stack[STACK_POINTER_INDEX] = 0;
    /* set stack_size to user-defined size */
    this_Stack[STACK_SIZE_INDEX] = stacksize;
    /* set stack_count to stack_counter, since new stack is always last added */
    this_Stack[STACK_COUNT_INDEX] = stack_counter;
    /* increment stack_counter after adding new stack */
    stack_counter++;

    /* if user wants debug msgs */
    if (debug)
    {
    	fprintf(stderr, ALLOCATED, (long)stack_counter);
    }

    /* return a pointer to beginning of stack */
    return this_Stack;
}

/*-----------------------------------------------------------------------------
Function Name:		get_occupancy
Purpose:		Returns the number of elements in the current stack.
Description:		Returns the index of the next available spot in the
			stack. Every spot before this one will be occupied,
			so the index will reflect the current capacity.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns a long representing the current capacity of the
			stack.
-----------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {
    /* check if stack exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, NUM_NONEXIST);	/* print nonexist err */
	return 0;
    }
    /* the next available index will be how many elements are already before it,
    or current size */
    return this_Stack[STACK_POINTER_INDEX];
}

/*-----------------------------------------------------------------------------
Function Name:		pop
Purpose:		Removes an item from the top of the stack, and sends it
			back through the output parameter item. 
Description:		If the stack isn't empty, store the index of the last
			element in the stack into a long. Then set the value of 
			item to the element, and decrement the stack pointer 
			index to overwrite the popped element.
Input:			Stack * this_Stack: A pointer to the current stack.
			long * item: A pointer to the popped item.
Result:			The popped element will be stored in item, and does not
			need to be returned. Returns TRUE upon success, and 
			FALSE upon failure.
-----------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
    /* check if stack exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, POP_NONEXIST);	/* print nonexist err */
	return 0;
    }
    /* check if input parameter exists */
    if (!item || !*item)
    {
    	fprintf(stderr, INCOMING_NONEXIST);	/* print nonexist err */
    	return 0;
    }
    /* make sure that stack isn't empty before pop */
    if (!isempty_Stack(this_Stack))
    {
    	long pos = this_Stack[STACK_POINTER_INDEX] - 1;	/* get index of item to
	pop */
	*item = this_Stack[pos];	/* set val of item to popped element */
	this_Stack[STACK_POINTER_INDEX]--;	/* decrement 
	stack_pointer_index to represent item has been popped */
    }
    /* if stack is already empty before pop, print err */
    else
    {
    	fprintf(stderr, POP_EMPTY);	/* print empty err */
	return 0;
    }
    /* if user wants debug msgs */
    if (debug)
    {
    	fprintf(stderr, POP, (long)stack_counter, *item);
    }

    return 1;
}

/*-----------------------------------------------------------------------------
Function Name:		push
Purpose:		Adds item to the top of the current stack.
Description:		If the stack isn't full, get the index of the next
			available position. Set the element at that position to
			item, then increment the stack pointer index.
Input:			Stack * this_Stack: A pointer to the current stack.
			long item: The element to be pushed onto the stack.
Result:			Returns TRUE upon success, and FALSE upon failure.
-----------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
    /* check if stack exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, PUSH_NONEXIST);	/* print nonexist err */
	return 0;
    }
    /* check if input parameter exists */
    if (!item)
    {
    	fprintf(stderr, INCOMING_NONEXIST);	/* print nonexist err */
    	return 0;
    }
    /* make sure that stack isn't full before push */
    if (!isfull_Stack(this_Stack))
    {
    	long pos = this_Stack[STACK_POINTER_INDEX];	/* get index of next
	available space */
	this_Stack[pos] = item;	/* push item into stack */
	this_Stack[STACK_POINTER_INDEX]++;	/* increment
	stack_pointer_index to represent item has been pushed */
    }
    /* if stack is already full before push, print err */
    else
    {
    	fprintf(stderr, PUSH_FULL);
	return 0;
    }
    /* if user wants debug msgs */
    if (debug)
    {
    	fprintf(stderr, PUSH, (long)stack_counter, item);
    }

    return 1;
}

/*-----------------------------------------------------------------------------
Function Name:		top
Purpose:		Sends back the item on the top of the stack through the
			output parameter item, but does not remove it from the
			stack. 
Description:		If the stack isn't empty, get the index of the last
			element in the stack. Then set item to the element at
			that index. Do not decrement the stack pointer index,
			because we do not want to alter the stack.
Input:			Stack * this_Stack: A pointer to the current stack.
			long * item: A pointer to the peeked element.
Result:			The peeked element will be returned through item, and
			does not need to be returned. Returns TRUE upon
			success, and FALSE upon failure.
-----------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
    /* check if stack exists */
    if (stack_counter == 0)
    {
    	fprintf(stderr, TOP_NONEXIST);	/* print nonexist err */
	return 0;
    }
    /* check if input parameter exists */
    if (!item || !*item)
    {
    	fprintf(stderr, INCOMING_NONEXIST);	/* print nonexist err */
    	return 0;
    }
    /* make sure that stack isn't empty before peek */
    if (!isempty_Stack(this_Stack))
    {
    	long pos = this_Stack[STACK_POINTER_INDEX] - 1;	/* get index of item to
	pop */
	*item = this_Stack[pos];	/* set val of item to popped element */
    }
    /* if stack is already empty before pop, print err */
    else
    {
    	fprintf(stderr, TOP_EMPTY);	/* print empty err */
	return 0;
    }
    /* if user wants debug msgs */
    if (debug)
    {
    	fprintf(stderr, POP, (long)stack_counter, *item);
    }

    return 1;
}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
