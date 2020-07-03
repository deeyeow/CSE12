/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/25/2020
                                                        cs12wi20fi
                                Assignment Eight

File Name:      Tree.c
Description:    This program will implement a binary tree data structure with a
                proper remove as well as a threshold check to maintain the 
                appropriate balance of the tree.

******************************************************************************/
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

/***************************************************************************
% Struct Name :  TNode
% File :         Tree.c
% 
% Description :  Represents a node within the overall tree structure. Each node
%                stores data regarding its height, balance, left and right
%                children pointers, and data to hold.
% 
% Data Fields : 
% 
% name           description
% ---------- -------------------------------------------------------------
% balance        The current balance of the TNode
% data           The data stored in the TNode
% height         The current height of the TNode
% *left          A pointer to the left child of the TNode
% *right         A pointer to the right child of the TNode
% occupancy      The current occupancy of the tree
% tree_count     The current tree count
%
% Functions :
%
% name           description
% ---------- -------------------------------------------------------------
% TNode          Constructor
% ~TNode         Individual destructor for each TNode
% delete_AllTNodes  Recursively deletes all TNodes of the tree
% ReplaceAndRemoveMax  Maintains the tree structure when deleting a TNode with
%                      two children
% Remove         Removes a specified TNode from the tree
% SetHeightAndBalance  Updates the height and balance of the current TNode
% Insert         Inserts a TNode into the tree
***************************************************************************/
template <class Whatever>
struct TNode {
        long balance;
        Whatever data;
        long height;
        TNode<Whatever> * left;
        long & occupancy;
        TNode<Whatever> * right;
        unsigned long & tree_count;
        
/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode()
% File :         Tree.c
% 
% Description :  Constructor for TNode
% 
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% element        The data to be stored in the TNode
% theTree        A reference to the tree that is being modified 
***************************************************************************/
        TNode (const Whatever & element, Tree<Whatever> & theTree)
                : balance (0), data (element), height (0), left (0), 
                occupancy (theTree.occupancy), right (0), 
                tree_count (theTree.tree_count) {

		/* increment occupancy */
		occupancy++;

        }
        
/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode()
% File :         Tree.c
% 
% Description :  Constructor for TNode
% 
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% element        The data to be stored in the TNode
% parentTNode    A reference to the parent of the created TNode
***************************************************************************/
        TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
                occupancy (parentTNode.occupancy), right (0), 
                tree_count (parentTNode.tree_count) {

		/* increment occupancy */
		occupancy++;
        }

/***************************************************************************
% Routine Name : TNode<Whatever> :: ~TNode()
% File :         Tree.c
% 
% Description :  Destructor for TNode
***************************************************************************/
        ~TNode (void) {

                /* decrement occupancy */
		occupancy--;
        }

/***************************************************************************
% Routine Name : TNode<Whatever> :: delete_AllTNodes()
% File :         Tree.c
% 
% Description :  Performs a post-order traversal through the Tree deleting each
%                TNode.
***************************************************************************/
        void delete_AllTNodes (void) {
                
		/* delete in post-order fashion, recurse left */
		if (left)
		{
			left->delete_AllTNodes();
		}

		/* recurse right */
		if (right)
		{
			right->delete_AllTNodes();
		}
		
		/* finally, delete curr TNode */
		delete(this);
        }

        unsigned long Insert (const Whatever & element, 
                              TNode<Whatever> *& PointerInParent);

        // OPTIONAL TNode :: Lookup => uncomment if implementing recursively
        // unsigned long Lookup(Whatever & element) const;

/***************************************************************************
% Routine Name : TNode<Whatever> :: ReplaceAndRemoveMax()
% File :         Tree.c
% 
% Description  : Called when removing a TNode with 2 children, replaces that
%                TNode with the maximum TNode in its left subtree to maintain 
%                the Tree structure.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% targetTNode    A reference to the TNode to remove that has 2 children
% PointerInParent  A reference to the TNode pointer in the parent TNode used to
%                  get to the current TNode
***************************************************************************/
        void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
                TNode<Whatever> *& PointerInParent) {
                
		if (Tree<Whatever>::debug)
		{
			cerr << TREE << tree_count << REPLACE << (const
			char*)data << "]" << endl;
		}

		/* keep recursing right until can't */
		if (right)
		{
			/* recurse and replace */
			right->ReplaceAndRemoveMax(targetTNode, right);
		}

		/* end of right-subtrees */
		else
		{
			/* update target TNode to right-most TNode in left
			 * subtree */
			targetTNode.data = this->data;

			/* set parent pointer back to left */
			PointerInParent = left;

			/* now can delete right-most TNode */
			delete(this);
		}
        }

/***************************************************************************
% Routine Name : TNode<Whatever> :: Remove()
% File :         Tree.c
% 
% Description  : Removes the matching data from the binary tree. Returns true or
%                false indicating success of removal.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% elementTNode   A reference to the TNode containing the data that identifies
%                the element to remove
% PointerInParent  A reference to the TNode pointer in the parent TNode used to
%                  get to the current TNode
% <return>       True or false indicating success of removal
***************************************************************************/
        unsigned long Remove (TNode<Whatever> & elementTNode, 
                TNode<Whatever> *& PointerInParent,
                long fromSHB = FALSE) {

		/* local variable to hold return value */
		long successOrNot = 0;

		if (Tree<Whatever>::debug)
		{
			cerr << TREE << tree_count << COMPARE <<
			(const char*)data << AND << 
			(const char*)elementTNode.data << "]" << endl;
		}

		/* if found TNode to remove */
		if (elementTNode.data == data)
		{
			/* update with data found in tree */
			elementTNode.data = data;

			/* if TNode to remove has both children */
			if (left && right)
			{
				if (Tree<Whatever>::debug)
				{
					cerr << TREE << tree_count << CHECK <<
					(const char*)left->data << "]" << endl;
				}

				/* remove TNode by traversing left subtree, and
				 * rebalance*/
				left->ReplaceAndRemoveMax(*this, left);
			}
			
			/* if TNode to remove has left child */
			else if (left)
			{
				/* set parent pointer to left */
				PointerInParent = left;
				delete(this);
			}

			/* if TNode to remove has right child */
			else if (right)
			{
				/* set parent pointer to right */
				PointerInParent = right;
				delete(this);
			}

			/* if TNode to remove has no children */
			else
			{
				/* dont set parent pointer, since no children */
				PointerInParent = NULL;
				delete(this);
			}

			return 1;
		}

		/* if didn't find TNode to remove */
		else
		{
			/* if TNode to remove is less than current TNode */
			if (data > elementTNode.data)
			{
				/* if left exists */
				if (left)
				{
					/* recurse using left TNode */
					successOrNot =
					left->Remove(elementTNode, left);
				}

				/* if left does't exist */
				else
				{
					return 0;
				}
			}

			/* if TNode to remove is greater than current TNode */
			else
			{
				/* if right exists */
				if (right)
				{
					/* recurse using right TNode */
					successOrNot =
					right->Remove(elementTNode, right);
				}

				/* if right doesn't exist */
				else
				{
					return 0;
				}
			}
		}

		/* if not from SetHeightAndBalance, can call function */
		if (!fromSHB)
		{
			SetHeightAndBalance(PointerInParent);
		}

		return successOrNot;
        }

/***************************************************************************
% Routine Name : TNode<Whatever> :: SetHeightAndBalance()
% File :         Tree.c
% 
% Description  : Updates the height and balance of the current TNode.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% PointerInParent  A reference to the TNode pointer in the parent TNode used to
%                  get to the current TNode
***************************************************************************/
        void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {

		/* local TNode to store removed data */
		TNode<Whatever> temp(data, *this);

		/* local variables to store left and right heights */
		long left_height = -1, right_height = -1;

		if (left)
		{
			left_height = left->height;
		}

		if (right)
		{
			right_height = right->height;
		}

		if (Tree<Whatever>::debug)
		{
			cerr << TREE << tree_count << UPDATE <<
			(const char*)data << "]" << endl;
		}

		/* find max height and update current TNode's height */
		height = std::max(left_height, right_height) + 1;

		/* also update current TNode's balance */
		balance = left_height - right_height;

		/* if tree is unbalanced at TNode */
		if (abs(balance) > THRESHOLD)
		{
			/* remove TNode with data */
			this->Remove(temp, PointerInParent, TRUE);

			/* insert new data into tree from parent */
			PointerInParent->Insert(temp.data, PointerInParent);
		}

        }

        ostream & Write_AllTNodes (ostream & stream) const {
                if (left)
                        left->Write_AllTNodes (stream);
                stream << *this;
                if (right)
                        right->Write_AllTNodes (stream);

                return stream;
        }
};

/***************************************************************************
% Routine Name : Tree<Whatever> :: Set_Debug_On()
% File :         Tree.c
% 
% Description :  Turns on debug
***************************************************************************/
template <class Whatever>
void Tree<Whatever>::Set_Debug_On()
{
	debug = 1;
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Set_Debug_Off()
% File :         Tree.c
% 
% Description :  Turns off debug
***************************************************************************/
template <class Whatever>
void Tree<Whatever>::Set_Debug_Off()
{
	debug = 0;
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Insert()
% File :         Tree.c
% 
% Description :  Inserts an element into the binary tree. Returns true or false
%                indicating success of insertion.
% 
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% element        The data to be stored in the TNode
% PointerInParent    A reference to the TNode pointer in parent TNode used to
%                    get to current TNode
% <return>       A long indicating success of insertion
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent) {

	/* iterate through tree until find an empty spot to insert*/
	if (Tree<Whatever>::debug)
	{
		cerr << TREE << tree_count << COMPARE <<
		(const char*)data << AND << 
		(const char*)element << "]" << endl;
	}

	/* if tree contains a duplicate, don't do anything and return true */
	if ((*data) == (*element))
	{
		 return 1;
	}

	/* if element is smaller than current TNode */
	else if ((*data) > (*element))
	{
		/* go left and insert if empty */
		if (!left)
		{
			if (Tree<Whatever>::debug)
			{
				cerr << TREE << tree_count << INSERT << 
				(const char*)element << "]" << endl;
			}

			left = new TNode<Whatever>(element, *this);
		}

		/* current TNode's left child is occupied,
		 * recurse until find a spot */
		else
		{
			left->Insert(element, left);
		}
	}

	/* if element is larger than current TNode */
	else
	{
		/* go right and insert if empty*/
		if (!right)
		{
			if (Tree<Whatever>::debug)
			{
				cerr << TREE << tree_count << INSERT <<
				(const char*)element << "]" << endl;
			}

			right = new TNode<Whatever>(element, *this);
		}

		/* current TNode's right child is occupied,
		 * recurse until find a spot */
		 else
		 {
			right->Insert(element, right);
		 }
	}

	/* balance height and balance of each inserted TNode */
	SetHeightAndBalance(PointerInParent);

	return 1;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: IsEmpty()
% File :         Tree.c
% 
% Description  : Returns whether the tree is empty or not.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% <return>       A boolean representing if the tree is empty or not
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: IsEmpty () const {
	
	return (occupancy == 0);
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Insert()
% File :         Tree.c
% 
% Description :  Inserts an element into the binary tree. Inserts at the root
%                TNode if Tree is empty, otherwise delegates to TNode's Insert.
%                Returns true or false indication success of insertion.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% element        The data to be stored in the TNode
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {

        /* if tree is empty, can insert at root */
	if (occupancy == 0)
	{
		if (debug)
		{
			cerr << TREE << tree_count << INSERT << 
			(const char*)element << "]" << endl;
		}

		/* create new TNode at root */
		root = new TNode<Whatever>(element, *this);
	}

	else
	{
		/* delegate to TNode's insert */
		return root->Insert(element, root);
	}
	return 0;
}

/***************************************************************************
% Routine Name : Tree<Whatever :: Lookup()
% File :         Tree.c
% 
% Description : This function will lookup the matching data in the binary tree.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element we are looking for.
% <return>           Returns true or false indicating success of look up.
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {

	/* if tree is empty */
	if (occupancy == 0)
	{
		return 0;
	}

	/* if tree already has elements, need to loop through, so keep
	 * track of current node */
	TNode<Whatever> *curr = root;
	
	/* iterate through tree until no more TNodes to search */
	while (curr)
	{
		if (debug)
		{
			cerr << TREE << tree_count << COMPARE <<
			(const char*)curr->data << AND << 
			(const char*)element << "]" << endl;
		}

		/* if element found, can return true */
		if ((*curr->data) == (*element))
		{
			/* set element to current TNode to return studetnum */
			element = curr->data;
			return 1;
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
        
	/* element not found */
	return 0;
}


/***************************************************************************
% Routine Name : Tree<Whatever> :: Remove()
% File :         Tree.c
% 
% Description :  Removes an element from the Tree. Delegates to TNode's Remove
%                when Tree is not empty. Returns true or false indicating
%                success of removal.
%                
% Parameter Descriptions : 
% 
% name           description
% ---------- -------------------------------------------------------------
% element        The data stored in the TNode to be removed
% <return>       True or false indicating success of removal
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {

	/* local variable to store return value from Tree's remove */
	long returnVal;

	/* store element in TNode so can pass to TNode's remove */
	TNode<Whatever> temp(element, *this);

        /* if tree not empty, delegate to TNode's remove */
	if (occupancy != 0)
	{
		returnVal = root->Remove(temp, root);
		element = temp.data;
		return returnVal;
	}

	return 0;
}
                                
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
% 
% Description :  Guarantee initialization of occupancy and root. It also 
%                initializes the tree_count using a static counter.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL) 

{        
        /* initialize counter and tree_count */
	static long counter = 0;
	tree_count = ++counter;

	if (debug)
	{
		cerr << TREE << tree_count << ALLOCATE;
	}
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{

	if (debug)
	{
		cerr << TREE << tree_count << DEALLOCATE << endl;
	}

	/* call TNode's delete_AllTNodes, starting at root */
	if (root)
	{
		root->TNode<Whatever>::delete_AllTNodes();
	}
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
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
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
