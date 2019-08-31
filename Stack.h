#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int col;
	int row;
}index;

typedef struct cellMem cellMem;

struct cellMem {
	index ix;
	int val;
	cellMem* next;
};

typedef struct {
	cellMem* top;
	int size;
}Stack;

/*initializes a stackNode and allocates memory.*/
cellMem* createNewNode(index ix, int val); 

/*push a new node to the top of the stack .*/
void push(index ix, int val, Stack *stack);

/*is the stack is empty- returns 1, else -0. .*/
int stackIsEmpty(Stack* stack);

/*pops the top of the stack and returns it*/
cellMem* pop(Stack* stack);

/*initialize a struct stack and it's veraibles. */
void initStack(Stack* stack);

#endif