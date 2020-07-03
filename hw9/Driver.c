/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        3/13/2020
                                                        cs12wi20fi
                                Assignment Nine

File Name:      Driver.c
Description:    This program is used to test our Tree data structure.

******************************************************************************/
#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"
#include <fstream>	//needed for class definition

using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
        return stream << "name:  " << stu.name
                << " with studentnum:  " << stu.studentnum;
}
/*******************************************************************
% Routine Name : int main (public)
% File :         Driver.c
% 
% Description :  Driver method for Tree.c.
%
% Parameters descriptions :
% 
% name		Description
% -------------- ---------------------------------------------------
% int argc:		The argument count of passed parameters.
% char * const * argv:	The character pointers to the parameters.
*******************************************************************/
int main (int argc, char * const * argv) {
        char buffer[BUFSIZ];
        char command;
        long number;
        char option;

	// initialize and declare input stream
	istream * inStream;
	inStream = &cin;
        
	// initialize and declare input stream
	ostream * outStream;
	outStream = &cout;

        SymTab<UCSDStudent>::Set_Debug_Off ();

        while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
                case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                        break;
                }       
        }
        
        SymTab<UCSDStudent> ST("Driver.datafile");
        ST.Write (cout << "Initial Symbol Table:\n" );

        while (cin) {

		// if input stream is EOF
		if (!*inStream && inStream != &cin)
		{
			// delete pointers
			delete inStream;
			delete outStream;

			// point back to cin and cout
			inStream = &cin;
			outStream = &cout;
		}

                command = NULL;         // reset command each time in loop
                *outStream << "Please enter a command ((f)ile, (i)nsert, "
                        << "(l)ookup, (r)emove, (w)rite):  ";
                *inStream >> command;

                switch (command) {
		// file case
		case 'f':
		{
			// prompt for filename
			*outStream << "Please enter file name for commands: ";
			*inStream >> buffer;

			// if input stream not pointing to cin
			if (inStream != &cin)
			{
				// delete pointers
				delete inStream;
				delete outStream;
			}

			// new ifstream pointing to file
			inStream = new ifstream(buffer);

			// new ofstream to dummy folder
			outStream = new ofstream("/dev/null");

			break;
		}

                case 'i': {
                        *outStream << 
			"Please enter UCSD student name to insert:  ";
                        *inStream >> buffer;  // formatted input

                        *outStream << "Please enter UCSD student number:  ";
                        *inStream >> number;

                        UCSDStudent stu (buffer, number);

                        // create student and place in symbol table
                        ST.Insert (stu);
                        break;
                }
                case 'l': { 
                        unsigned long found;    // whether found or not

                        *outStream << 
			"Please enter UCSD student name to lookup:  ";
                        *inStream >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        found = ST.Lookup (stu);
                        
                        if (found)
                                cout << "Student found!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                        }
                case 'r': { 
                        unsigned long removed;

                        *outStream << 
			"Please enter UCSD student name to remove:  ";
                        *inStream >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        removed = ST.Remove(stu);

                        if (removed)
                                cout << "Student removed!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                }
                case 'w':
                        ST.Write (cout << "The Symbol Table contains:\n");
                }
        }

        ST.Write (cout << "\nFinal Symbol Table:\n");

	if (ST.GetOperation() != 0) 
	{
        	cout << "\nCost of operations: ";
        	cout << ST.GetCost();
        	cout << " tree accesses";

        	cout << "\nNumber of operations: ";
        	cout << ST.GetOperation();

        	cout << "\nAverage cost: ";
        	cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
        	cout << " tree accesses/operation\n";
	}

	else
        	cout << "\nNo cost information available.\n";

}
