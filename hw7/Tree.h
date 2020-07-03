#ifndef TREE_H
#define TREE_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

#include <iostream>
#include "Base.h"
using namespace std;

struct TNode;

class Tree : public Base {
	TNode * root;
	const char * tree_name;
	static bool debug_on;

public:
	static void Set_Debug (bool);
	Tree (const char *);
	~Tree (void);
	unsigned long Insert (Base *);
	unsigned long IsEmpty () const;
	const Base * Lookup (const Base *) const;
	Base * Remove (const Base *);
	ostream & Write (ostream &) const ;
};

#endif
