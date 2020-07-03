#ifndef MYLIB_H
#define MYLIB_H

/* DO NOT ADD CODE TO THIS FILE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.  Changing it in other ways will cause
 * you deductions or a score of 0 on your assignment.
 * YOU HAVE BEEN WARNED!!!
 */

void clrbuf (int);
long decin (void);
void decout (long);
void hexout (unsigned long);
void newline (void);
long writeline (char *);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef NULL
#undef NULL
#define NULL 0
#endif

#endif
