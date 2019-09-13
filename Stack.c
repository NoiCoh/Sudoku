#include "Stack.h"
#include "MainAux.h"

/*
* the function initializes a stack node and allocates memory.
* return value: initialized stack node.
*/
cellMem* createNewNode(index ix, int val) {
	cellMem* stackNode = (cellMem*)malloc(sizeof(cellMem));
	if (!stackNode) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
	stackNode->ix = ix;
	stackNode->val = val;
	stackNode->next = NULL;
	return stackNode;
}

/*
* the function pushes a new node to the top of the stack
*/
void push(index ix, int val, Stack* stack) {
	cellMem *newCell, *prevTop;
	prevTop = stack->top;
	newCell = createNewNode(ix, val);
	newCell->next = prevTop;
	stack->top = newCell;
	stack->size++;
}

/*
* the function checks if the stack is empty.
* return value: if the stack is empty- returns 1, else -0
*/
int stackIsEmpty(Stack* stack) {
	if (stack->size == 0) {
		return 1;
	}
	return 0;
}

/* 
* the function pops the top of the stack and returns it.
*/
cellMem* pop(Stack* stack) {
	cellMem* prevTop;
	prevTop = stack->top;
	stack->top = stack->top->next;
	prevTop->next = NULL;
	stack->size--;
	return prevTop;
}

/*
* the function initializes a struct stack and it's veraibles.
*/
void initStack(Stack* stack) {
	stack->top = NULL;
	stack->size = 0;
}