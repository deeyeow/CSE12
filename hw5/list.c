/****************************************************************************

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/2/2020
                                                        cs12wi20fi
                                Assignment Five

File Name:      list.c
Description:    A polymorphic generic container based on a linked-list data 
		structure. In list.c, the List and Node structs are defined. 
		The List object contains data fields such as occupancy and 
		list_count, and also contains pointers to functions which 
		manipulate the objects stored in the list. The Node objects 
		form a doubly-linked list, where the pre pointer points to the 
		previous item in the list and the next pointer points to the 
		next item in the list. The list is circular, so the next 
		pointer of the last item points to the first item of the list,
		and the pre pointer of the first item points to the last item 
		in the list. We keep track of the front item in the list with 
		the front data field of List.


1. front: 0x6040f0
2. Node 1 data: 0x6040d0
3. Node 1 pre: 0x604070
4. Node 1 next: 0x6040b0
5. Node 2 data: 0x604090
6. Node 2 pre: 0x6040f0
7. Node 2 next: 0x604070
8. Node 3 data: 0x604050
9. Node 3 pre: 0x6040b0
10: Node 3 next: 0x6040f0

******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

typedef struct List {
        Node * front;             /* the front of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        long (*is_greater_than_func) (void *, void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static int locate (List * this_list, void * element);
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
                "Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
                "Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
                "Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
                "Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
                "Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
                "Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
                "Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
                "Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
                "Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
                "Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
                "Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
                "Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*-----------------------------------------------------------------------------
Function Name:		set_debug_on
Purpose:		Turns on debugging for this list.
Description:		Sets debug_on to TRUE.
Input:			Nothing.
Result:			Debug_on is set. Nothing is returned.
-----------------------------------------------------------------------------*/
void set_debug_on (void) {
    debug_on = TRUE;
}

/*-----------------------------------------------------------------------------
Function Name:		set_debug_off
Purpose:		Turns off debugging for this list.
Description:		Sets debug_on to FALSE.
Input:			Nothing.
Result:			Debug_on is set. Nothing is returned.
-----------------------------------------------------------------------------*/
void set_debug_off (void) {
    debug_on = FALSE;
}

/*-----------------------------------------------------------------------------
Function Name:		advance_next_List
Purpose:		Advances the list forward by 1 node.
Description:		Calling this function causes the front pointer of 
			this_list to move forward by one Node. This effectively
			shifts the elements of the list forward by one.
Input:			List * this_list: A pointer to the list.
Result:			Nothing is returned.
-----------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
    
    /* if list null, print nonexist msg */
    if (!this_list)
    {
    	fprintf(stderr, ADNEXT_NONEXIST);
	return;
    }

    /* if list empty, print empty msg */
    if (isempty_List(this_list))
    {
    	fprintf(stderr, ADNEXT_EMPTY);
	return;
    }

    /* if debug on, print msg */
    if (debug_on)
    {
    	fprintf(stderr, ADNEXT, this_list -> list_count);
    }

    /* advance list pointer */
    this_list -> front = this_list -> front -> next;
    return;
}

/*-----------------------------------------------------------------------------
Function Name:		advance_pre_List
Purpose:		Advances the list backward by 1 node.
Description:		Calling this function causes the front pointer of 
			this_list to move backward by one Node. This effectively
			shifts the elements of the list backward by one.
Input:			List * this_list: A pointer to the list.
Result:			Nothing is returned.
-----------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {

    /* if list null, print nonexist msg */
    if (!this_list)
    {
    	fprintf(stderr, ADPRE_NONEXIST);
	return;
    }

    /* if list empty, print empty msg */
    if (isempty_List(this_list))
    {
    	fprintf(stderr, ADPRE_EMPTY);
	return;
    }

    /* if debug on, print msg */
    if (debug_on)
    {
    	fprintf(stderr, ADPRE, this_list -> list_count);
    }

    /* de-advance list pointer */
    this_list -> front = this_list -> front -> pre;
    return;
}

/*-----------------------------------------------------------------------------
Function Name:		delete_List
Purpose:		Deletes a list.
Description:		This destructor function deallocates all memory 
			associated with the list, including the memory 
			associated with all of the nodes in the list. It sets 
			the list pointer in the calling function to NULL.
Input:			List ** lpp: A pointer to a pointer to the list.
Result:			Nothing is returned.
-----------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
    
    List * this_list;
    void * data;
    Node * temp;
    long list_num;

    /* get *lpp */
    this_list = *lpp;
    
    /* save which list was deleted */
    list_num = this_list -> list_count;

    /* check if list exists */
    if (!lpp || !*lpp || this_list -> list_count == 0)
    {
    	fprintf(stderr, DELETE_NONEXIST);
	return;
    }

    /* delete all nodes in loop, until occupancy is 0 */
    while (!isempty_List(this_list))
    {
    	temp = this_list -> front -> next;	/* might accidentally delete
	front node, so need to move */
    	data = remove_Node(this_list -> front); /* delete node */
	this_list -> delete_func(&data); /* delete data */
	this_list -> front = temp;	/* reset front to temp */
	this_list -> occupancy--;	/* decrement occupancy */
    }

    /* deallocate list pointer */
    free(*lpp);

    /* set pointer to null */
    *lpp = 0;

    /* decrement list_counter */
    list_counter--;

    /* print debug msg */
    if (debug_on)
    {
    	fprintf(stderr, LIST_DEALLOCATE, list_num);
    }
}

/*-----------------------------------------------------------------------------
Function Name:		insert
Purpose:		Inserts a node into the list.
Description:		Inserts the element into this_list either at the front,
			end or in sorted form. locate should be used to locate
			where the node should go in the list. This function
			should then call insert_Node to incorporate the Node
			into the list.
Input:			List * this_List: A pointer to the list.
			void * element: A pointer to the element to be inserted.
			long where: Specifies how the user wants to insert the
			element.
Result:			A long signifying success is returned.
-----------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
    
    Node * temp;
    
    /* keep track if front pointer changed */
    int front_pointer_changed;

    /* save the current front pointer in this_list */
    temp = this_list -> front;

    /* check if list exists */
    if (!this_list || this_list -> list_count == 0)
    {
    	fprintf(stderr, INSERT_NONEXIST);
	return 0;
    }

    /* print debug msg */
    if (debug_on)
    {
    	fprintf(stderr, INSERT, this_list -> list_count);
    }

    /* if list is empty, can insert node to front */
    if (isempty_List(this_list))
    {
    	/* insert to front, then set front pointer to inserted node */
    	this_list -> front = insert_Node(this_list -> front, element, 
	this_list -> copy_func);
    }

    /* if list has elements, need to decide where to insert */
    else
    {
    	switch(where)
	{
		/* if user wants to insert to front */
		case 1:
			/* insert to front, then set front pointer to inserted
			 * node */
			this_list -> front = insert_Node(this_list -> front,
			element, this_list -> copy_func);
			break;
		/* if user wants to insert to back */
		case 0:
			/* insert before front pointer, but don't update front 
			pointer, so inserted node is actually at the end of 
			 * list */
			insert_Node(this_list -> front, element, this_list ->
			copy_func);
			break;
		/* if user wants to insert sorted */
		case 2:
			front_pointer_changed = locate(this_list, element);
			
			/* if front was changed */
			if (front_pointer_changed)
			{
				insert_Node(this_list -> 
				front, element, this_list -> copy_func);

				/* reset front pointer */
				this_list -> front = temp;
			}

			/* if front was not changed */
			else
			{
				this_list -> front = insert_Node(this_list ->
				front, element, this_list -> copy_func);
			}
			break;
	}
    }
    this_list -> occupancy++;	/* increment occupancy */

    return 1;
}

/*-----------------------------------------------------------------------------
Function Name:		locate
Purpose:		Locates where a node should be inserted in sort mode.
Description:		locate should be used to locate where the node should
			be in a sorted list. If your implementation of locate 
			changes the front pointer of the list, be sure to save 
			that value before calling this function.
Input:			List * this_list: A pointer to the list.
			void * element: A pointer to the element to be inserted.
Result:			An int signifying if the front pointer was changed is
			returned.
-----------------------------------------------------------------------------*/
static int locate (List * this_list, void * element) {

    int count = 0;

    /* move pointer forward until reach node that has a larger value, or reach
     * end of list */
    while (count < this_list -> occupancy && this_list ->
    is_greater_than_func(element, this_list -> front -> data))
    {
    	advance_next_List(this_list);
	count++;
    }

    /* check if front pointer was changed in list, and needs to be reset in
     * insert */
    if (count == 0)
    {
    	return 0;
    }
    else
    {
    	return 1;
    }
}
 
/*-----------------------------------------------------------------------------
Function Name:		isempty_List
Purpose:		Check whether the list is empty.
Description:		Check to see if this_list is empty.
Input:			List * this_list: A pointer to the list.
Result:			A long signifying if the list is empty.
-----------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
    
    /* check if list exists */
    if (!this_list || this_list -> list_count == 0)
    {
    	fprintf(stderr, ISEMPTY_NONEXIST);
	return 1;
    }

    /* check list occupancy */
    if (this_list -> occupancy == 0)
    {
    	return 1;
    }

    return 0;
}

/*-----------------------------------------------------------------------------
Function Name:		new_List
Purpose:		Creates a new list.
Description:		This constructor function allocates and initializes a 
			new List object. It initializes the list data fields, 
			and returns a pointer to the list. All new lists should
			be empty.
Input:			void *(*copy_func): A pointer to the function
			which makes copies of the elements stored in this_list.
			void (*delete_func): A pointer to the function which 
			frees the memory associated with elements stored in 
			this_list.
			long (*is_greater_than_func): A pointer to the function
			which compares elements in this_list.
			FILE *(*write_func): A pointer to the function which
			writes elements in this_list.
Result:			A pointer to the created list.
-----------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {
        
    List * this_List = (List *) malloc(sizeof(List));	/* allocate memory for
    list */

    list_counter++;	/* increment static list counter */

    /* set object data to constructor parameters */
    this_List -> front = 0;
    this_List -> occupancy = 0;
    this_List -> list_count = list_counter;
    this_List -> copy_func = copy_func;
    this_List -> delete_func = delete_func;
    this_List -> is_greater_than_func = is_greater_than_func;
    this_List -> write_func = write_func;

    /* print debug msg */
    fprintf(stderr, LIST_ALLOCATE, this_List->list_count);

    return this_List;
}

/*-----------------------------------------------------------------------------
Function Name:		remove_List
Purpose:		Removes an element from the list at a user-specified
			location.
Description:		Removes an element in this_list at location where. This
			function should call remove_Node to restructure the 
			list to remove the node.
Input:			List * this_list: A pointer to the list.
			long where: A long specifying where the user wants to
			remove a node.
Result:			A pointer to the data of the removed node.
-----------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {

    /* store return value of remove_Node */
    void * data;

    /* check if list exists */
    if (!this_list)
    {
    	fprintf(stderr, REMOVE_NONEXIST);
	return 0;
    }

    /* check if list is empty */
    if (isempty_List(this_list))
    {
    	fprintf(stderr, REMOVE_EMPTY);
	return 0;
    }

    /* print debug msg */
    if (debug_on)
    {
    	fprintf(stderr, REMOVE, this_list -> list_count);
    }

    /* decrement occupancy */
    this_list -> occupancy--;

    /* remove front */
    if (where)
    {
    	/* need to change front pointer, or else will accidentally delete */
    	this_list -> front = this_list -> front -> next;
    	data = remove_Node(this_list -> front -> pre);
    }

    /* remove end */
    else
    {
    	/* don't need to change front pointer, since deleting from end */
    	data =  remove_Node(this_list -> front -> pre);
    }

    /* if list is empty, remember to reset front to NULL */
    if (isempty_List(this_list))
    {
    	this_list -> front = 0;
    }

    return data;
}

/*-----------------------------------------------------------------------------
Function Name:		view
Purpose:		Views either the front or end of the list.
Description:		Returns a pointer to the object stored at location 
			where for viewing. This function should call view_Node,
			which returns the data.
Input:			List * this_list: A pointer to the list.
			long where: A long specifying which node the user wants
			to view.
Result:			A pointer to the data of the node.
-----------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
    
    /* check if list exists */
    if (!this_list)
    {
    	fprintf(stderr, VIEW_NONEXIST);
	return 0;
    }

    /* check if list is empty */
    if (isempty_List(this_list))
    {
    	fprintf(stderr, VIEW_EMPTY);
	return 0;
    }

    /* print debug msg */
    if (debug_on)
    {
    	fprintf(stderr, VIEW, this_list -> list_count);
    }
    /* print front */
    if (where)
    {
    	return view_Node(this_list -> front);
    }

    /* print end */
    else
    {
    	return view_Node(this_list -> front -> pre);
    }
}

FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);

        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        if (this_list->occupancy >= 1)
                working = this_list->front;

        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
                
        return stream;
}

/*-----------------------------------------------------------------------------
Function Name:		write_reverse_List
Purpose:		Writes elements of the list in reverse order.
Description:		Writes the elements of this_list backwards, starting 
			with the last item. The list is printed to filestream 
			stream.
Input:			List * this_list: A pointer to the list.
			FILE * stream: A pointer to the stream to print to.
Result:			A stream containing the desired output.
-----------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {

    long count;	/*know how many elements to print */
    Node * working;	/* working node */

    /* check nonexistent stream */
    if (!stream)
    {
    	fprintf(stderr, WRITE_NONEXISTFILE);
	return NULL;
    }

    /* check nonexistent list */
    if (!this_list)
    {
    	fprintf(stderr, WRITE_NONEXISTLIST);
	return NULL;
    }

    /* print premiliminary msg */
    if (stream == stderr)
    {
    	fprintf(stream, "List %ld has %ld items in it.\n",
	this_list->list_count, this_list->occupancy);
    }

    /* check null write_func function */
    if (!this_list->write_func)
    {
    	fprintf(stream, WRITE_MISSINGFUNC);
	return stream;
    }

    /* if list has multiple elements, save front pointer */
    if (this_list->occupancy >= 1)
    {
    	working = this_list->front;
    }

    /* iterate and print from the back */

    working = working->pre;	/* set front to last */
    for (count = this_list->occupancy; count > 0; count--)
    {
    	if (stream == stderr)
	{
		fprintf(stream, "\nelement %ld: ", count);
	}
	write_Node(working, stream, this_list->write_func);
	working = working->pre;
    }

    return stream;
}

static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}

/*-----------------------------------------------------------------------------
Function Name:		insert_Node
Purpose:		Insert a node into the list.
Description:		Creates a new node to hold element, or, if copy_func is
			non-NULL, a copy of element. This new node is then
			incorporated into the list at the location BEFORE
			this_Node.
Input:			Node * this_Node: A pointer to the node that the new
			node is inserted before.
			void * element: A pointer to the object we wish to store
			in the new node.
			void * (*copy_func): A pointer to the copy function
			passed into the list constructor or NULL if no copy
			needs to be made.
Result:			A pointer to the newly inserted node.
-----------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
        void * (*copy_func) (void *)) {

    /* if no nodes yet, create new node to hold data, then set pointers to 
    itself */
    if (!this_Node)
    {
    	Node * temp = new_Node(element, copy_func);
	temp -> next = temp;
	temp -> pre = temp;
	return temp;
    }

    /* if already node in list, insert to beginning */
    else
    {
    	Node * temp = new_Node(element, copy_func);	/* create node to be
	inserted */

	/* attach new node into list */
	temp -> next = this_Node;	/* set new node next to first node */
	temp -> pre = this_Node -> pre;	/* set new node pre to last node */

	/* integrate new node */
	this_Node -> pre -> next = temp;	/* set last node next to new
	node */
	this_Node -> pre = temp;	/* set first node pre to new node */

	return temp;
    }
}

static Node* new_Node (void * element, void * (*copy_func) (void *)) {

        /* allocate memory */
        Node *this_Node = (Node *) malloc (sizeof (Node));

        /* initialize memory */
        this_Node->next = this_Node->pre = NULL;
        this_Node->data = (copy_func) ? (*copy_func) (element) : element;

        return this_Node;
}

/*-----------------------------------------------------------------------------
Function Name:		remove_Node
Purpose:		Remove a node from the list.
Description:		"Unlinks" this_Node from the list by arranging the 
			pointers of the surrounding Nodes so they no longer 
			point to this_Node. The memory associated with the Node
			object is freed, but the Node's data is not deleted. A 
			pointer to the data is returned.
Input:			Node * this_Node: A pointer to the node we wish to
			remove from the list.
Result:			A pointer to the data of the removed node.
-----------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {

    /* save node data */
    Node * temp;
    temp = this_Node -> data;
    
    /* check if anything to remove */
    if (!this_Node)
    {
    	return 0;
    }

    /* rearrange node pointers, so it is no longer in the list */
    this_Node -> next -> pre = this_Node -> pre;
    this_Node -> pre -> next = this_Node -> next;
    
    /* delete the node */
    delete_Node(&this_Node, 0);
    return temp;
}

/*-----------------------------------------------------------------------------
Function Name:		view_Node
Purpose:		Returns a pointer to a node's data..
Description:		Returns a pointer to this_Node's data field.
Input:			Node * this_Node: A pointer to the node whose data field
			we wish to view.
Result:			A pointer to the data of the given node.
-----------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
    
    /* check if node exists */
    if (!this_Node)
    {
    	fprintf(stderr, VIEW_NONEXISTNODE);
	return 0;
    }

    return this_Node -> data;
}

static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
