#include "Stack.h"

void push(index ix, int val, Stack* stack) {
	cellMem *newCell, *prevTop;
	prevTop = stack->top;
	newCell = createNewNode(ix, val);
	newCell->next = prevTop;
	stack->top = newCell;
	stack->size++;
}

cellMem* createNewNode(index ix, int val) {
	cellMem* stackNode = (cellMem*)malloc(sizeof(cellMem));
	stackNode->ix = ix;
	stackNode->val = val;
	stackNode->next = NULL;
	return stackNode;
}

int stackIsEmpty(Stack* stack) {
	if (stack->size == 0) {
		return 1;
	}
	return 0;
}

cellMem* pop(Stack* stack) {
	cellMem* prevTop;
	prevTop = stack->top;
	stack->top = stack->top->next;
	prevTop->next = NULL;
	stack->size--;
	return prevTop;
}

void initStack(Stack* stack) {
	stack->top = NULL;
	stack->size = 0;
}