#ifndef STACK_H
#define STACK_H

/* DO NOT ADD CODE TO THIS FILE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.  Changing it in other ways will cause
 * you deductions or a score of 0 on your assignment.
 * YOU HAVE BEEN WARNED!!!
 */

#include <stdio.h>
#include "list.h"

typedef struct List Stack;

void delete_Stack (Stack **);
long isempty_Stack (Stack *);
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *));
void * pop (Stack *);
long push (Stack *, void * element);
void * top (Stack *);
FILE * write_Stack (Stack *, FILE *);

#endif
