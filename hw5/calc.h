#ifndef CALC_H
#define CALC_H

/* DO NOT ADD CODE TO THIS FILE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.  Changing it in other ways will cause
 * you deductions or a score of 0 on your assignment.
 * YOU HAVE BEEN WARNED!!!
 */

#include <stdio.h>
#include "stack.h"

/* declare public functions for stack words */
void delete_CalcWord (void *);
FILE * write_CalcWord (void *, FILE *);

/* basic calculator functions */
long eval (Stack *);
long intopost (Stack *);

#endif
