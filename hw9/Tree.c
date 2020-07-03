/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        3/13/2020
                                                        cs12wi20fi
                                Assignment Nine

File Name:      Tree.c
Description:    This program will implement a binary tree data structure with a
                proper remove as well as a threshold check to maintain the 
                appropriate balance of the tree. It can also read and write to a
		file.

******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

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
% Routine Name : TNode (struct) 
% File :         Tree.c
% 
% Description :  Represents a node within the overall tree structure. Each node
%                stores data regarding its height, balance, left and right
%                children pointers, and data to hold.
***************************************************************************/
template <class Whatever>
struct  TNode {
// friends:

// data fields:
	Whatever data;
	long height;
	long balance;
	offset left;
	offset right;
	offset this_position;	// current position

// function fields:
	TNode () : height (0), balance (0), left (0), right (0), 
		this_position (0) {}

	// to declare the working TNode in Tree's Remove
	TNode (Whatever & element) : data (element), height (0), balance (0),
		left (0), right (0), this_position (0) {}
	
	TNode (Whatever &, fstream *, long &);	// to add new node to disk
	TNode (const offset &, fstream *);	// to read node from disk
	
	unsigned long Insert (Whatever &, fstream *, long &, offset &);
	// optional recursive Lookup declaration would go here
	void Read (const offset &, fstream *);	// read node from disk
	unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
		long fromSHB = FALSE);
	void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
	void SetHeightAndBalance (fstream *, offset &);
	void Write (fstream *) const;		// update node to disk

	ostream & Write_AllTNodes (ostream &, fstream *) const;
};

/***************************************************************************
% Routine Name : Tree<Whatever> :: Insert (public) 
% File :         Tree.c
% 
% Description :  Inserts an element into the binary tree. Inserts at the root
%                TNode if Tree is empty, otherwise delegates to TNode's Insert.
%                Returns true or false indication success of insertion.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% Whatever & element:			 Data to be stored in the inserted TNode
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element) {
	
	// if tree is empty, can insert at root
	if (occupancy == 0)
	{
		//increment operation
		IncrementOperation();
		
		// create temp write TNode
		TNode<Whatever> tempWriteNode(element, fio, occupancy);
		// update new root
		root = tempWriteNode.this_position;

		return true;
	}
	// tree is already populated
	else
	{
		// increment operation
		IncrementOperation();
		
		//create temp read TNode
		TNode<Whatever> tempReadNode(root, fio);

		// delegate to TNode's insert
		return tempReadNode.Insert(element, fio, occupancy, root);
	}
	
	return 0;
}

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
% TNode<Whatever> & targetTNode:A reference to the TNode to remove that has 2
% 				children.
% fstream * fio:	Filestream to find the tree on disk.
% PointerInParent:	Reference to the position of the pointer in parent.
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
	fstream * fio, offset & PositionInParent) {
		
		// keep recursing right until can't
		if (right)
		{
			// local temp right node
			TNode<Whatever> tempRightNode(right, fio);
			// recurse and replace
			tempRightNode.ReplaceAndRemoveMax(targetTNode, fio,
			right);
			// set height and balance
			SetHeightAndBalance(fio, PositionInParent);
		}

		// end of right-subtrees
		else
		{
			// update target TNode to right-most TNode in left
			// subtree
			targetTNode.data = this->data;

			// set parent pointer back to left
			PositionInParent = left;
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
% TNode<Whatever> & elementTNode:A reference to the TNode containing the data 
% 				that identifies the element to remove.
% fstream * fio:	Filestream to find the tree on disk.
% PointerInParent:	Reference to the position of the pointer in parent.
% long fromSHB:		Boolean to keep track if Remove was called from SHAB.
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
	fstream * fio, long & occupancy, offset & PositionInParent,
	long fromSHB) {
	
		// local variable to hold return value
		long successOrNot = 0;

		// if found TNode to remove
		if (elementTNode.data == data)
		{
			// update with data found in tree
			elementTNode.data = data;

			// if TNode to remove has both children
			if (left && right)
			{
				// remove node by traversing left subtree
				TNode<Whatever> leftNode(left, fio);
				leftNode.ReplaceAndRemoveMax(*this, fio, left);

				// rebalance if not from SHAB
				if (!fromSHB)
				{
					SetHeightAndBalance(fio,
					PositionInParent);
				}
				// if already SHAB, write to file
				else
				{
					Write(fio);
				}
			}
			
			// if TNode to remove has left child
			else if (left)
			{
				// set parent pointer to left
				PositionInParent = left;
			}

			// if TNode to remove has right child
			else if (right)
			{
				// set parent pointer to right
				PositionInParent = right;
			}

			// if TNode to remove has no children
			else
			{
				// dont set parent pointer, since no children
				PositionInParent = 0;
			}

			return 1;
		}

		// if didn't find TNode to remove
		else
		{
			// if TNode to remove is less than current TNode
			if (data > elementTNode.data)
			{
				// local left node to remove
				TNode<Whatever> removeLeftNode(left, fio);
				// if left exists
				if (left)
				{
					// recurse using left TNode
					successOrNot =
					removeLeftNode.Remove(elementTNode, fio,
					occupancy, left, fromSHB);
				}

				// if left does't exist
				else
				{
					return 0;
				}
			}

			// if TNode to remove is greater than current TNode
			else
			{
				// local right node to remove
				TNode<Whatever> removeRightNode(right, fio);
				// if right exists
				if (right)
				{
					// recurse using right TNode
					successOrNot =
					removeRightNode.Remove(elementTNode,
					fio, occupancy, right, fromSHB);
				}

				// if right doesn't exist
				else
				{
					return 0;
				}
			}
		}

		// if not from SetHeightAndBalance, can call function
		if (!fromSHB && successOrNot)
		{
			SetHeightAndBalance(fio, PositionInParent);
		}

		return successOrNot;
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
	
	// local variable to store return value from Tree's remove
	long returnVal;

	// local temp node to remove
	TNode<Whatever> tempNode(element);

        // if tree not empty
	if (occupancy != 0)
	{
		// local temp root node
		TNode<Whatever> removeRoot(root, fio);
		// delegate to TNode's remove
		returnVal = removeRoot.Remove(tempNode,fio, occupancy, root, 0);

		// store removed node's data
		element = tempNode.data;
		
		// if removed node was root, reset root
		if (occupancy == 1)
		{
			ResetRoot();
		}

		//decrement occupancy
		occupancy--;

		return returnVal;
	}

	// increment operation
	IncrementOperation();
	
	return 0;
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
% fstream * fio:		Filestream to find the tree on disk.
% offset & PositionInParent:	Reference to the position of the pointer in
% 				parent.
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
	offset & PositionInParent) {

		// local variables to store left and right heights
		long left_height = -1, right_height = -1;

		// get left height
		if (left)
		{
			TNode<Whatever> leftHeight(left, fio);
			left_height = leftHeight.height;
		}

		// get right height
		if (right)
		{
			TNode<Whatever> rightHeight(right, fio);
			right_height = rightHeight.height;
		}

		// find max height and update current TNode's height
		height = std::max(left_height, right_height) + 1;

		// also update current TNode's balance
		balance = left_height - right_height;

		// if tree is unbalanced at TNode
		if (abs(balance) > THRESHOLD)
		{
			// local variable to not mess with tree in file
			long fakeOccupancy = 0;
			// local node with data
			TNode<Whatever> tempNode(data);
			// remove TNode with data
			Remove(tempNode, fio, fakeOccupancy, PositionInParent,
			TRUE);
			// local node for insertion position
			TNode<Whatever> tempInsertNode(PositionInParent, fio);
			// insert new data into tree from parent
			tempInsertNode.Insert(tempNode.data, fio, fakeOccupancy,
			PositionInParent);
		}

		// if tree is balanced
		else
		{
			Write(fio);
		}
}

/***************************************************************************
% Routine Name : Tree <Whatever> :: GetCost (public) 
% File :         Tree.c
% 
% Description :  Returns the cost.
% 
% name               description
% ------------------ -------------------------------------------------------
% <return>:				Returns the cost.
***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetCost () {
	
	return cost;
}

/***************************************************************************
% Routine Name : Tree <Whatever> :: GetOperation (public) 
% File :         Tree.c
% 
% Description :  Returns the cost.
% 
% name               description
% ------------------ -------------------------------------------------------
% <return>:				Returns the operation.
***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetOperation () {
	
	return operation;
}

/***************************************************************************
% Routine Name : Tree <Whatever> :: IncrementCost (public) 
% File :         Tree.c
% 
% Description :  Increments the cost.
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementCost () {
	
	//increment cost
	cost++;
}

/***************************************************************************
% Routine Name : Tree <Whatever> :: IncrementOperation (public) 
% File :         Tree.c
% 
% Description :  Increments the operation.
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementOperation () {
	
	//increment operation
	operation++;
}

/***************************************************************************
% Routine Name : Tree <Whatever> :: ResetRoot (public) 
% File :         Tree.c
% 
% Description :  Resets the root to the end of file.
***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: ResetRoot () {
	
	// goto beginning
	fio->seekp(0, ios::beg);
	//create space for new root
	fio->write((const char*)&root, sizeof(root));
	// update root value
	root = fio->tellp();
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Insert (public) 
% File :         Tree.c
% 
% Description :  Inserts an element into the binary tree. Returns true or false
% 		 indicating success of insertion.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% Whatever & element:			 Data to be stored in TNode.
% fstream * fio:			 Filestream to find the Tree on disk.
% long & occupancy:			 Occupancy of the tree.
% offset & PositionInParent:		 Reference to the position of the
% 					 pointer in parent.
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
	long & occupancy, offset & PositionInParent) {

	// if tree contains a duplicate
	if ((*data) == (*element))
	{
		// overwrite curr TNode data
		data = element;
		//write to file
		Write(fio);
		return 1;
	}

	// if element is smaller than current TNode
	else if ((*data) > (*element))
	{
		// go left and insert if empty
		if (!left)
		{
			// create local write node
			TNode<Whatever> tempWriteNode(element, fio, occupancy);
			// update position of left node
			left = tempWriteNode.this_position;
			// update height and balance for new Tree
			SetHeightAndBalance(fio, PositionInParent);

			return 1;
		}

		// current TNode's left child is occupied,
		// recurse until find a spot
		else
		{
			// create local read node
			TNode<Whatever> tempReadNode(left, fio);
			//recurse
			tempReadNode.Insert(element, fio, occupancy, left);
		}
	}

	// if element is larger than current TNode
	else
	{
		// go right and insert if empty
		if (!right)
		{
			// create local write node
			TNode<Whatever> tempWriteNode(element, fio, occupancy);
			//update position of right node
			right = tempWriteNode.this_position;
			// update height and balance for new Tree
			SetHeightAndBalance(fio, PositionInParent);

			return 1;
		}

		// current TNode's right child is occupied,
		// recurse until find a spot
		 else
		 {
		 	// create local read node
			TNode<Whatever> tempReadNode(right, fio);
			// recurse
			tempReadNode.Insert(element, fio, occupancy, right);
		 }
	}

	// call SHAB again 
	SetHeightAndBalance(fio, PositionInParent);

	return 1;
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
	
	// goto end of file
	fio->seekg(0, ios::end);
	//get location of read pointer
	offset ending = fio->tellg();

	// if tree is empty
	if (root == ending || occupancy == 0)
	{
		return 0;
	}

	// if tree already has elements, need to loop through, so keep
	// track of current node
	TNode<Whatever> curr(root, fio);
	
	// iterate through tree until no more TNodes to search
	while (curr.data)
	{
		// if element found, can return true
		if ((curr.data) == (element))
		{
			// set element to current TNode to return studetnum
			element = curr.data;
			// increment operation
			IncrementOperation();

			return 1;
		}

		// if element is smaller than current TNode
		else if ((curr.data) > (element))
		{
			// go left and check if empty
			if (!curr.left)
			{
				// increment operation
				IncrementOperation();

				return 0;
			}

			// current TNode's left child is occupied,
			// update curr and keep looping down tree
			else
			{
				// local temp left node for recursion
				TNode<Whatever> tempLeftNode(curr.left, fio);
				curr = tempLeftNode;
				continue;
			}
		}

		// if element is larger than current TNode
		else
		{
			// go right and check if empty
			if (!curr.right)
			{
				// increment operation
				IncrementOperation();

				return 0;
			}

			// current TNode's right child is occupied,
			//  update curr and keep looping down tree
			 else
			 {
			 	// local temp right node for recursion
				TNode<Whatever> tempRightNode(curr.right, fio);
				curr = tempRightNode;
				continue;
			 }
		}
	}
        
	// element not found
	return 0;
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Read (public) 
% File :         Tree.c
% 
% Description :  Reads a TNode from file into memory.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% const offset & position:		 Offset to find the TNode in file.
% fstream * fio:			 Filestream to find the Tree on disk.
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) {
	
	//move read pointer to position
	fio->seekp(position);
	//overwrite "this" TNode's data with read in data
	fio->read((char*)this, sizeof(TNode<Whatever>));

	//increment cost
	Tree<Whatever>::IncrementCost();

	//debug message
	if (Tree<Whatever>::debug_on)
	{
		cerr << COST_READ << (const char*)data << ']' << endl;
	}
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode (public) 
% File :         Tree.c
% 
% Description :  TNode read constructor.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% Whatever & element:			Data to be stored in the new TNode. 
% fstream * fio:			Filestream to find the Tree on disk.
% long & occupancy:			Occupancy of the Tree.
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) {
	
	//calls Read method to read in from file
	Read(position, fio);
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode (public) 
% File :         Tree.c
% 
% Description :  TNode write constructor.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% Whatever & element:			Data to be stored in the new TNode. 
% fstream * fio:			Filestream to find the Tree on disk.
% long & occupancy:			Occupancy of the Tree.
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy): 
			data (element), height (0), balance (0), left (0), 
			right (0) {
	
	//move write pointer to end of file
	fio->seekp(0, ios::end);
	//update curr position
	this_position = fio->tellp();

	//write to file
	Write(fio);

	//increment occupancy
	occupancy++;
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Write (public) 
% File :         Tree.c
% 
% Description :  Writes a TNode from memory into file.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% fstream * fio:			 Filestream to find the Tree on disk.
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const {
	
	//goto current position
	fio->seekp(this_position);
	//write data to file
	fio->write((const char*)this, sizeof(TNode<Whatever>));

	//increment cost
	Tree<Whatever>::IncrementCost();

	//debug message
	if (Tree<Whatever>::debug_on)
	{
		cerr << COST_WRITE << (const char*)this << ']' << endl;
	}
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree (public) 
% File :         Tree.c
% 
% Description :  Allocates the tree object. If the file is empty, root and 
		 occupancy fields are written to the file. If file is already
		 populated, read root and occupancy into memory.
%
% Parameters descriptions :
% 
% name               description
% ------------------ -------------------------------------------------------
% const char * datafile:		 The filestream to find Tree on disk.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
	fio (new fstream (datafile, ios :: out | ios :: in)) {
	
	// initialize static count
	static int count = 0;

	// increment tree_count
	tree_count = ++count;

	// move fio pointers, and initialize offset accordingly
	fio->seekg(0, ios::beg);
	offset beginning = fio->tellg();

	fio->seekg(0, ios::end);
	offset ending = fio->tellg();

	// check if file is empty
	if (beginning == ending)
	{
		// reset root
		root = 0;
		//reset occupancy
		occupancy = 0;
		// move write pointer back to beginning of file
		fio->seekp(0, ios::beg);
		// write to file
		fio->write((const char*)&root, sizeof(root));
		fio->write((const char*)&occupancy, sizeof(occupancy));
		// set root to offset value
		root = fio->tellp();
	}

	// if file not empty
	else
	{
		// need to read, move read pointer to beginning of file
		fio->seekg(0, ios::beg);
		// read in
		fio->read((char*)&root, sizeof(root));
		fio->read((char*)&occupancy, sizeof(occupancy));
	}
	

	// debug message
	if (debug_on)
	{
		cerr << TREE << tree_count << ALLOCATE;
	}
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.
***************************************************************************/

{
	// move read pointer to beginning of file
	fio->seekp(0, ios::beg);

	// write root to file
	fio->write((const char*)&root, sizeof(root));

	// write occupancy to file
	fio->write((const char*)&occupancy, sizeof(occupancy));

	// delete stream object
	delete fio;

	// debug message
	if (debug_on)
	{
		cerr << TREE << tree_count << DEALLOCATE << endl;
	}

	// decrement tree_count
	tree_count--;
}	/* end: ~Tree */

/***************************************************************************
% Routine Name : Tree<Whatever> :: Set_Debug_On()
% File :         Tree.c
% 
% Description :  Turns on debug
***************************************************************************/
template <class Whatever>
void Tree<Whatever>::Set_Debug_On()
{
	debug_on = 1;
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
	debug_on = 0;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This funtion will output the contents of the Tree table
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
        long old_cost = cost;

	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	fio->seekg (0, ios :: end);
	offset end = fio->tellg ();

	// check for new file
	if (root != end) {
		TNode<Whatever> readRootNode (root, fio);
		readRootNode.Write_AllTNodes (stream, fio);
	}

        // ignore cost when displaying nodes to users
        cost = old_cost;

	return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
	if (left) {
		TNode<Whatever> readLeftNode (left, fio);
		readLeftNode.Write_AllTNodes (stream, fio);
	}
	stream << *this;
	if (right) {
		TNode<Whatever> readRightNode (right, fio);
		readRightNode.Write_AllTNodes (stream, fio);
	}

	return stream;
}

