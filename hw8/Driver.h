/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/18/2020
                                                        cs12wi20fi
                                Assignment Eight

File Name:      Driver.c
Description:    This program is used to define the UCSDStudent class, and test
                our Tree data structure.

******************************************************************************/

#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

/***************************************************************************
% Class Name :   UCSDStudent
% File :         Driver.h
% 
% Description :  Represents a UCSDStudent object to be inserted in the data
%                field of TNodes in our tree.
% 
% Data Fields : 
% 
% name           description
% ---------- -------------------------------------------------------------
% name           The name of our object
% studentnum     The student ID of our object
%
% Functions :
%
% name           description
% ---------- -------------------------------------------------------------
% UCSDStudent    Constructor
% operator const char *  Overloaded operator to print the name of our object
% operator ==    Overloaded operator to compare 2 objects
% operator >     Overloaded operator to check if one object is greater than the
%                other
***************************************************************************/
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[20];
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
		studentnum (num) 
		{
			strcpy(name, nm);
		}

/***************************************************************************
% Routine Name : UCSDStudent  (public)
% File :         Driver.c
% 
% Description :  A copy constructor for UCSDStudent.
%
% Parameter descriptions :
%
% name              description
% ------------ ------------------------------------------------------------
% student           A reference to the student being copied
***************************************************************************/
	UCSDStudent (const UCSDStudent & student)
	{
		strcpy(name, student.name);
		studentnum = student.studentnum;
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
	long operator == (const UCSDStudent & bbb) const 
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
	long operator > (const UCSDStudent & bbb) const
	{
		return (strcmp (name, bbb) > 0) ? 1 : 0;
	}

	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  studentnum:  " << studentnum;
	}


};

#endif
