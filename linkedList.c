#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include "MainAux.h"

/*
* initializes node and it's variables.
* return value: initialize node.
*/
node* initializeNode(int row,  int col,  int newVal,  int prevVal) {
	node* newNode;
	newNode = (node*)malloc(sizeof(node));
	if (newNode == NULL) {
		funcFailed("malloc");
	}
	newNode->row = row;
	newNode->col = col;
	newNode->newVal = newVal;
	newNode->prevVal = prevVal;
	newNode->next = NULL;
	return newNode;
}

/*
* initializes linkedList and returns it.
*/
linkedList* initializeLinkedList() {
	linkedList* newList;
	newList = (linkedList*)calloc(1, sizeof(linkedList));
	if (newList == NULL) {
		funcFailed("calloc");
	}
	return newList;
}

/*
* The function checks if the linked list is empty.
* return value : if the linked list is empty-returns 1, otherwise - 0.
*/
int isEmpty(linkedList *list) {
	if (list->size == 0) {
		return 1;
	}
	return 0;
}

/*
* inserts a node to the end of the linked list.
*/
void insertLast(linkedList *list,int row,  int col,  int Newval,  int prevVal) {
	node *newNode, *head, *tail, * lastNode;
	newNode = initializeNode(row, col, Newval, prevVal);
	head = list->head;
	tail = list->tail;
	if (isEmpty(list)) {
		list->head = newNode;
	}
	else {
		lastNode = tail;
		if (tail == NULL) {
			lastNode = head;
		}
		lastNode->next = newNode;
		list->tail = newNode;
	}
	list->size++;
}

/*
* deletes the first node of the linked list .
*/
void deleteFirst(linkedList* list) {
	node* next, *head;
	head = list->head;

	if (!isEmpty(list)) {
		next = head->next;
		list->head = next;
		free(head);
		list->size--;
		if (list->size <= 1) {
			list->tail = NULL;
		}
	}
}

/*
* frees memory resources allocated for the linked list.
*/
void freeList(linkedList* list) {
	while (!isEmpty(list)) {
		deleteFirst(list);
	}
	free(list);
}

/*
* initializes a doubly node for the doubly linked list.
* return value : initialized doubly node
*/
doublyNode* initializeDoublyNode(linkedList *move) {
	doublyNode* newNode;
	newNode = (doublyNode*)calloc(1, sizeof(doublyNode));
	if (newNode == NULL) {
		funcFailed("calloc");
	}
	newNode->move = move;
	return newNode;
}

/*
* initializes a doubly linked list and returns it.
*/
doublyLinkedList* initializeDoublyLinkedList() {
	doublyLinkedList* newNode;
	newNode = (doublyLinkedList*)calloc(1, sizeof(doublyLinkedList));
	if (newNode == NULL) {
		funcFailed("calloc");
	}
	return newNode;
}

/*
* The function checks if the linked list is empty.
* return value : if the doubly linked list is empty-returns 1, otherwise - 0.
*/
int doublyIsEmpty(doublyLinkedList *list) {
	if (list->size == 0) {
		return 1;
	}
	return 0;
}

/*
* inserts a node to the end of the doubly linked list.
*/
void doublyInsertLast(doublyLinkedList *list, linkedList *move) {
	doublyNode *newNode, *head, *tail, * prevNode;
	newNode = initializeDoublyNode(move);
	head = list->head;
	tail = list->tail;
	if (doublyIsEmpty(list)) {
		list->head = newNode;
	}
	else {
		prevNode = tail;
		if (tail == NULL) { 
			prevNode = head;
		}
		prevNode->next = newNode;
		newNode->prev = prevNode;
		list->tail = newNode;
	}
	list->size++;
}

/*
* deletes all nodes after the curMove pointer.
*/
void doublyDeleteAllAfter(doublyLinkedList* list, doublyNode *curMove) {
	doublyNode* tmp;
	tmp = doublyGetLast(list);
	if (tmp != NULL) {
		while (tmp != curMove) {
			doublyDeleteLast(list);
			tmp = doublyGetLast(list);
		}
	}
}

/*
* the function gets the last node in the doubly linked list and returns it.
*/
doublyNode* doublyGetLast(doublyLinkedList* list) {
	if (doublyIsEmpty(list)) {
		return NULL;
	}
	if (list->tail == NULL) {
		return list->head;
	}
	return list->tail;
}

/*
* The function deletes the last node of the doubly linked list.
*/
void doublyDeleteLast(doublyLinkedList *list) {
	doublyNode *last, *prev;
	last = doublyGetLast(list);

	if (!doublyIsEmpty(list)) {
		prev = last->prev;
		list->tail = prev;
		if (prev != NULL) { 
			prev->next = NULL;
		}
		if (last->move != NULL) {
			freeList(last->move);
		}
		free(last);

		list->size--;
		if (list->size <= 1) {
			list->tail = NULL;
		}
		if (list->size == 0) {
			list->head = NULL;
		}
	}
}

/*
* frees memory resources allocated for the doubly linked list.
*/
void doublyFree(doublyLinkedList* list) {
	while (!doublyIsEmpty(list)) {
		doublyDeleteLast(list);
	}
	if (list != NULL) {
		free(list);
	}
}
