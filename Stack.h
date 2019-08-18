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


cellMem* createNewNode(index ix, int val); 

void push(index ix, int val, Stack *stack);

int stackIsEmpty(Stack* stack);

cellMem* pop(Stack* stack);

void initStack(Stack* stack);

#endif