/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/18/2020
                                                        cs12wi20fi
                                Assignment Seven

File Name:      Tree.c
Description:    This program will implement a binary tree data structure. This
                structure will be used to allow variable assignment in the 
		calculator.

******************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

struct TNode {
	Base * data;
	TNode * left, * right, *parent;
	static long occupancy;
        unsigned long hasBeenDeleted;

	// left child's height - right child's height
	long balance;

	// 1 + height of tallest child, or 0 for leaf
	long height;

	TNode (Base * element) : data (element), left (0), right (0),
	        parent (0), hasBeenDeleted (FALSE), balance (0), height(0) {
/***************************************************************************
% Routine Name : TNode  (public)
% File :         Tree.c
% 
% Description :  A constructor for TNode.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% element           The element to store in the data variable.
***************************************************************************/

		//increment occupancy when adding new node
		occupancy++;

	}
	~TNode (void) {
/***************************************************************************
% Routine Name : ~TNode  (public)
% File :         Tree.c
% 
% Description :  A destructor for TNode.
***************************************************************************/
		// delete data in TNode
		delete data;

		// set pointers to null
		parent = NULL;
		left = NULL;
		right = NULL;

		// decrement occupancy
		occupancy--;
	}
        ostream & Write (ostream & stream) const {
                stream << "at height:  " << height << "  with balance:  "
			<< balance << "  ";
		return data->Write (stream) << "\n";
        }
	ostream & Write_AllTNodes (ostream & stream) const {
		if (left)
			left->Write_AllTNodes (stream);
		if (!hasBeenDeleted)
			Write (stream);
		if (right)
			right->Write_AllTNodes (stream);

		return stream;
	}

	void delete_AllTNodes (void) {
/***************************************************************************
% Routine Name : delete_AllTNodes  (public)
% File :         Tree.c
% 
% Description :  A destructor to delete all TNodes in a tree.
***************************************************************************/
		// recursively delete all nodes, in post-order fashion
		if (right)
		{
			right->delete_AllTNodes();
		}

		if (left)
		{
			left->delete_AllTNodes();
		}

		delete this;
	}
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

void Tree :: Set_Debug (bool option) {
/***************************************************************************
% Routine Name : Set_Debug  (public)
% File :         Tree.c
% 
% Description :  Sets the debug flag.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% option            The value to set debug_on to.
***************************************************************************/

	// toggle debug 
	debug_on = option;
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/

{
	// just print debug msg
	if (debug_on)
	{
		cerr << TREE << tree_name << ALLOCATE << endl;
	}

}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	// start delete_AllTNodes at root, if there is one
	if (root)
	{
		root->delete_AllTNodes();
	}

	// free pointer to tree
	free((void*) (tree_name));

}	/* end: ~Tree */

unsigned long Tree :: IsEmpty () const {
/***************************************************************************
% Routine Name : IsEmpty  (public)
% File :         Tree.c
% 
% Description :  Returns if the tree is empty.
***************************************************************************/

	return TNode::occupancy == 0;
}

unsigned long Tree :: Insert (Base * element) {
/***************************************************************************
% Routine Name : Insert  (public)
% File :         Tree.c
% 
% Description :  Inserts a TNode into the tree at the appropriate location.
%                Updates height and balance of each TNode accordingly.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% element           A reference to the element we are inserting.
***************************************************************************/

	/* if tree is empty */
	if (!root)
	{
		/* set root to new TNode with element */
		root = new TNode(element);
		
		if (debug_on)
		{
			cerr << TREE << tree_name << INSERT << 
			(const char*) (*element) << "]" << endl;
		}

		return TRUE;
	}

	/* if tree already has elements, need to loop through, so keep
	 * track of current node */
	TNode *curr = root;
	
	/* iterate through tree until find an empty spot to insert*/
	while (curr)
	{
		if (debug_on)
		{
			cerr << TREE << tree_name << COMPARE <<
			(const char*) (*curr->data) << AND << 
			(const char*) (*element) << "]" << endl;
		}

		/* if tree contains a duplicate */
		if ((*curr->data) == (*element))
		{
			/* if duplicate was deleted, can copy element
			 * into TNode at location */
			if (curr->hasBeenDeleted)
			{
				/* storing parent node */
				TNode *prev = curr;

				/* copying data into new TNode */
				curr = new TNode(element);
				curr->parent = prev->parent;
				curr->left = prev->left;
				curr->right = prev->right;
				curr->balance = prev->balance;
				curr->height = prev->height;

				/* setting delete flag to false */
				curr->hasBeenDeleted = FALSE;
			}

			/* if duplicate not deleted, don't do anything
			 * to tree */
			 break;
		}

		/* if element is smaller than current TNode */
		else if ((*curr->data) > (*element))
		{
			/* go left and insert if empty */
			if (!curr->left)
			{
				if (debug_on)
				{
					cerr << TREE << tree_name << INSERT << 
					(const char*) (*element) << "]" << endl;
				}

				curr->left = new TNode(element);
				curr->left->parent = curr;
				//curr.left.height = curr.height + 1;

				break;
			}

			/* current TNode's left child is occupied,
			 * update curr and keep looping down tree */
			else
			{
				curr = curr->left;
				continue;
			}
		}

		/* if element is larger than current TNode */
		else
		{
			/* go right and insert if empty*/
			if (!curr->right)
			{
				if (debug_on)
				{
					cerr << TREE << tree_name << INSERT <<
					(const char*) (*element) << "]" << endl;
				}

				curr->right = new TNode(element);
				curr->right->parent = curr;
				//curr.right.height = curr.height + 1;

				break;
			}

			/* current TNode's right child is occupied,
			 * update curr and keep looping down tree */
			 else
			 {
				curr = curr->right;
				continue;
			 }
		}
	}

	/* update balance and height by looping back up tree */
	while (curr)
	{
		/* initialize local variables to keep track of and
		 * compare heights */
		long leftHeight = 0, rightHeight = 0;

		/* if current TNode has left child, update leftHeight */
		if (curr->left)
		{
			leftHeight = curr->left->height + 1;
		}

		/* if current TNode has right child, update rightHeight */
		if (curr->right)
		{
			rightHeight = curr->right->height + 1;
		}

		/* find max height and update current TNode's height */
		curr->height = std::max(leftHeight, rightHeight);

		/* also update current TNode's balance */
		curr->balance = leftHeight - rightHeight;

		/* loop up the tree */
		curr = curr->parent;
	}

	return TRUE;
}

const Base * Tree :: Lookup (const Base * element) const {
/***************************************************************************
% Routine Name : Tree :: Lookup (public)
% File :         Tree.c
% 
% Description : This function will lookup the matching data in the binary tree.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element we are looking for.
% <return>           A reference to the matching data in the binary tree.
***************************************************************************/

	/* if tree is empty */
	if (!root)
	{
		return 0;
	}

	/* if tree already has elements, need to loop through, so keep
	 * track of current node */
	TNode *curr = root;
	
	/* iterate through tree until no more TNodes to search */
	while (curr)
	{
		if (debug_on)
		{
			cerr << TREE << tree_name << COMPARE <<
			(const char*) (*curr->data) << AND << 
			(const char*) (*element) << "]" << endl;
		}

		/* if tree contains a duplicate */
		if ((*curr->data) == (*element))
		{
			/* if duplicate was deleted, no TNode to be found in
			 * tree */
			if (curr->hasBeenDeleted)
			{
				return 0;
			}

			/* if duplicate not deleted, can return TNode */
			else
			{
				return curr->data;
			}
		}

		/* if element is smaller than current TNode */
		else if ((*curr->data) > (*element))
		{
			/* go left and check if empty */
			if (!curr->left)
			{
				return 0;
			}

			/* current TNode's left child is occupied,
			 * update curr and keep looping down tree */
			else
			{
				curr = curr->left;
				continue;
			}
		}

		/* if element is larger than current TNode */
		else
		{
			/* go right and check if empty*/
			if (!curr->right)
			{
				return 0;
			}

			/* current TNode's right child is occupied,
			 * update curr and keep looping down tree */
			 else
			 {
				curr = curr->right;
				continue;
			 }
		}
	}

	/* not there */
	return 0;
}

Base * Tree :: Remove (const Base * element) {
/***************************************************************************
% Routine Name : Tree :: Remove (public)
% File :         Tree.c
% 
% Description : This function will remove the matching data from the binary tree
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element we are removing.
% <return>           A reference to the removed data in the binary tree.
***************************************************************************/

	/* if tree is empty */
	if (!root)
	{
		return 0;
	}

	/* if tree already has elements, need to loop through, so keep
	 * track of current node */
	TNode *curr = root;
	
	/* iterate through tree until no more TNodes to search */
	while (curr)
	{
		if (debug_on)
		{
			cerr << TREE << tree_name << COMPARE <<
			(const char*) (*curr->data) << AND << 
			(const char*) (*element) << "]" << endl;
		}

		/* if tree contains a duplicate */
		if ((*curr->data) == (*element))
		{
			/* if duplicate was deleted, no TNode to be found in
			 * tree */
			if (curr->hasBeenDeleted)
			{
				return 0;
			}

			/* if duplicate not deleted, can return TNode and
			 * "delete" */
			else
			{
				/* set delete flag to true */
				curr->hasBeenDeleted = TRUE;

				/* decrement occupancy */
				TNode::occupancy--;

				return curr->data;
			}
		}

		/* if element is smaller than current TNode */
		else if ((*curr->data) > (*element))
		{
			/* go left and check if empty */
			if (!curr->left)
			{
				return 0;
			}

			/* current TNode's left child is occupied,
			 * update curr and keep looping down tree */
			else
			{
				curr = curr->left;
				continue;
			}
		}

		/* if element is larger than current TNode */
		else
		{
			/* go right and check if empty*/
			if (!curr->right)
			{
				return 0;
			}

			/* current TNode's right child is occupied,
			 * update curr and keep looping down tree */
			 else
			 {
				curr = curr->right;
				continue;
			 }
		}
	}

	/* not there */
	return 0;
}

ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */

