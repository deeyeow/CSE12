/****************************************************************************

                                                        Darren Yau
                                                        CSE 12, W20
                                                        1/21/2020
                                                        cs12wi20fi
                                Assignment Five

File Name:      stack.c
Description:    An array-based stack of longs. The user can create, delete, 
		and modify stacks through push, pop, and empty functions. The
		user can also check if a stack is empty or full, as well as 
		peek at the top element and get the current capacity.
****************************************************************************/
#include <stdio.h>
#include "list.h"
#include "stack.h"

/*-----------------------------------------------------------------------------
Function Name:		delete_Stack
Purpose:		This destructor function deletes a stack.
Description:		Calls delete_List to delete the stack.
Input:			Stack **spp: A pointer to a pointer to the stack.
Result:			Nothing.
-----------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}

/*-----------------------------------------------------------------------------
Function Name:		isempty_Stack
Purpose:		Checks if the current stack is empty or not.
Description:		Calls isempty_List to check if the stack is empty.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns TRUE if the stack is empty, FALSE if the stack
			is not empty.
-----------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}

/*-----------------------------------------------------------------------------
Function Name:		new_Stack
Purpose:		This constructor function allocates and initializes a
			new Stack object.
Description:		Creates a new stack by calling the new_List constructor.
Input:			void *(*copy_func): A pointer to the copy function. 
			void (*delete_func): A pointer to the delete function.
			long (*is_greater_than_func): A pointer to the
			is_greater_than function.
			FILE *(*write_func): A pointer to the stream to send the
			desired output to.
Result:			Returns a pointer to the memory address of the new 
			stack.
-----------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, 
		is_greater_than_func, write_func);
}

/*-----------------------------------------------------------------------------
Function Name:		pop
Purpose:		Removes an item from the top of the stack, and sends it
			back through the output parameter item. 
Description:		Calls remove_List and passes in the end of the stack as
			a parameter.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns a pointer to the popped item.
-----------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}

/*-----------------------------------------------------------------------------
Function Name:		push
Purpose:		Adds item to the top of the current stack.
Description:		Calls insert and passes in the element and the end of
			the stack as parameters.
Input:			Stack * this_Stack: A pointer to the current stack.
			void * element: A pointer to the element to be pushed 
			onto the stack.
Result:			Returns TRUE upon success, and FALSE upon failure.
-----------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}

/*-----------------------------------------------------------------------------
Function Name:		top
Purpose:		Sends back the item on the top of the stack through the
			output parameter item, but does not remove it from the
			stack. 
Description:		Calls view and passes in the end of the stack as a
			parameter.
Input:			Stack * this_Stack: A pointer to the current stack.
Result:			Returns a pointer to the peeked item.
-----------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}
/*-----------------------------------------------------------------------------
Function Name:		write_Stack
Purpose:		Writes out the contents of the stack.
Description:		Calls write_List and passes in the stream as a
			parameter.
Input:			Stack * this_Stack: A pointer to the current stack.
			FILE * stream: A pointer to the stream that displays the
			desired output.
Result:			Returns the stream where the output is stored.
-----------------------------------------------------------------------------*/

FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
