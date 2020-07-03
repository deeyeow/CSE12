/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/18/2020
                                                        cs12wi20fi
                                Assignment Seven

File Name:      Driver.c
Description:    This program is used to define the UCSDStudent class, and test
                our Tree data structure.

******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string.h>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

class UCSDStudent : public Base {
	char * name;
	long studentnum;
public:

/***************************************************************************
% Routine Name : UCSDStudent  (public)
% File :         Driver.c
% 
% Description :  A constructor for UCSDStudent.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% nm                A reference to the name of UCSDStudent to be set.
% num               A long of the num of UCSDStudent to be set.
***************************************************************************/
	UCSDStudent (char * nm, long num = 0) :
		name (strdup (nm)), studentnum (num) {}

/***************************************************************************
% Routine Name : ~UCSDStudent  (public)
% File :         Driver.c
% 
% Description :  A destructor for UCSDStudent.
***************************************************************************/
	~UCSDStudent (void)
	{
		free (name);
	}

/***************************************************************************
% Routine Name : operator const char *  (public)
% File :         Driver.c
% 
% Description :  An overloaded operator to return the name of UCSDStudent.
***************************************************************************/
	operator const char * (void) const 
	{
		return name;
	}

/***************************************************************************
% Routine Name : long operator ==  (public)
% File :         Driver.c
% 
% Description :  An overloaded operator to return object equality.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% bbb               A reference to the object to check equality against the
%                   current UCSDStudent.
***************************************************************************/
	long operator == (const Base & bbb) const 
	{
		return ! strcmp (name, bbb);
	}

/***************************************************************************
% Routine Name : long operator >  (public)
% File :         Driver.c
% 
% Description :  An overloaded operator to check if an object is greater than
%                the current UCSDStudent.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% bbb               A reference to the object to compare against the
%                   current UCSDStudent.
***************************************************************************/
	long operator > (const Base & bbb) const
	{
		return (strcmp (name, bbb) > 0) ? 1 : 0;
	}

	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  studentnum:  " << studentnum;
	}
};

int main (int argc, char * const * argv) {
	char buffer[80];
	char command;
	long number;

	Tree::Set_Debug(0);

	if (argc != 1 && strcmp("-x", argv[1]) == 0) {
		Tree::Set_Debug(1);
	}
	
	SymTab * ST;
	ST = new SymTab ("UCSDStudentTree");
	ST->Write (cout << "Initial Symbol Table:\n");

	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<  "Please enter a command:  ((a)llocate, is(e)mpty," <<
			" (i)nsert, (l)ookup, (r)emove, (w)rite):  ";
		cin >> command;

		switch (command) {

		case 'a':
			cout << "Please enter name of new Tree to allocate:  ";
			cin >> buffer; 

			delete ST;
			ST = new SymTab (buffer);

			break;

		case 'e':
                        if (ST->IsEmpty ())
                                cout << "Tree is empty." << endl;
                        else
                                cout << "Tree is not empty." << endl;
                        break;
			
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			ST->Insert (new UCSDStudent (buffer, number));
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST->Lookup (&stu);
			
			if (found)
				found->Write (cout << "Student found!\n") << 
				"\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;
		
		case 'r': {
			Base * removed;	// data to be removed

			cout << "Please enter UCSD student name to remove:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			removed = ST->Remove (&stu);

			if (removed)
				removed->Write (cout << "Student removed!\n") <<
				"\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;


		case 'w':
			ST->Write (cout << "The Symbol Table contains:\n");
		}
	}

	ST->Write (cout << "\nFinal Symbol Table:\n");
	delete ST;
}
