#ifndef TREE_H
#define TREE_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

#include <iostream>
using namespace std;

// debug message
static const char ALLOCATE[] = " - Allocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
struct TNode;

/***************************************************************************
% Class Name :   Tree
% File :         Tree.h
% 
% Description :  Represents a binary tree of individual TNodes.
% 
% Data Fields : 
% 
% name           description
% ---------- -------------------------------------------------------------
% height         The current height of the TNode
% *root          A pointer to the root node of the tree
% occupancy      The current occupancy of the tree
% tree_count     The current tree count
% debug          Debug flag
%
% Functions :
%
% name           description
% ---------- -------------------------------------------------------------
% Tree           Constructor
% ~Tree          Destructor
% Set_Debug_On   Sets debug flag to true
% Set_Debug_Off  Sets debug flag to false
% IsEmpty        Returns if the tree is empty or not
% Insert         Inserts the root into the tree, or calls TNode's Insert if tree
%                already has elements
% Lookup         Returns if a specified element is found in the tree
% Remove         Removes a specified TNode from the tree by calling TNode's
%                Remove
% Write          Writes the tree to the specified stream
***************************************************************************/
template <class Whatever>
class Tree {
        friend struct TNode<Whatever>;
        long occupancy;
        TNode<Whatever> * root;
        unsigned long tree_count;
        static int debug;
public:
        
        Tree (void);
        ~Tree (void);
        
        static void Set_Debug_On (void);
        static void Set_Debug_Off (void);
        unsigned long IsEmpty (void) const;
        unsigned long Insert (const Whatever &);
        unsigned long Lookup (Whatever &) const;
        unsigned long Remove (Whatever &);
        ostream & Write (ostream &) const;
};

#include "Tree.c"

#endif
