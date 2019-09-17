/*
 * Stack:
 * This module implements a stack.
 */
#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>

/*
* the struct represents a index of a board cell - colunm and row
*/
typedef struct {
	int col;
	int row;
}index;

/*
* the struct represents a cell of the struct Stack with all the information needed for a recursive call.
*/
typedef struct cellMem cellMem;

struct cellMem {
	index ix;
	int val;
	cellMem* next;
};

/*
* this struct represents a stack of cellMem
*/
typedef struct {
	cellMem* top;
	int size;
}Stack;

/*
* the function initializes a stack node and allocates memory.
* return value: initialized stack node.
*/
cellMem* createNewNode(index ix, int val); 

/*
* the function pushes a new node to the top of the stack
*/
void push(index ix, int val, Stack *stack);

/*
* the function checks if the stack is empty.
* return value: if the stack is empty- returns 1, else -0
*/
int stackIsEmpty(Stack* stack);

/*
* the function pops the top of the stack and returns it.
*/
cellMem* pop(Stack* stack);

/*
* the function initializes a struct stack and it's veraibles.
*/
void initStack(Stack* stack);

#endif