#ifndef SYMTAB_H
#define SYMTAB_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

#include <iostream>
#include "Base.h"
#include "Hash.h"
using namespace std;

class SymTab : private HashTable {
public:
	using HashTable :: Insert;
	using HashTable :: Lookup;
	using HashTable :: Write;
	SymTab (int sz) : HashTable (sz) {}
};

#endif

