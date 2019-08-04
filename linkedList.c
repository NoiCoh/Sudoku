#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include "MainAux.h"

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

linkedList* initializeLinkedList() {
	linkedList* newNode;
	newNode = (linkedList*)calloc(1, sizeof(linkedList));
	if (newNode == NULL) {
		funcFailed("calloc");
	}
	return newNode;
}


int isEmpty(linkedList *list) {
	if (list->size == 0) {
		return 1;
	}
	return 0;
}

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

void freeList(linkedList* list) {
	while (!isEmpty(list)) {
		deleteFirst(list);
	}
	free(list);
}

doublyNode* initializeDoublyNode(linkedList *move) {
	doublyNode* newNode;
	newNode = (doublyNode*)calloc(1, sizeof(doublyNode));
	if (newNode == NULL) {
		funcFailed("calloc");
	}
	newNode->move = move;
	return newNode;
}

doublyLinkedList* initializeDoublyLinkedList() {
	doublyLinkedList* newNode;
	newNode = (doublyLinkedList*)calloc(1, sizeof(doublyLinkedList));
	if (newNode == NULL) {
		funcFailed("calloc");
	}
	return newNode;
}

int doublyIsEmpty(doublyLinkedList *list) {
	if (list->size == 0) {
		return 1;
	}
	return 0;
}

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

doublyNode* doublyGetLast(doublyLinkedList* list) {
	if (doublyIsEmpty(list)) {
		return NULL;
	}
	if (list->tail == NULL) {
		return list->head;
	}
	return list->tail;
}

void doublyDeleteLast(doublyLinkedList *list) {
	doublyNode *last, *prev;
	last = doublyGetLast(list);

	if (!doublyIsEmpty(list)) {
		prev = last->prev;
		list->tail = prev;
		if (prev != NULL) { 
			prev->next = NULL;
		}
		freeList(last->move);
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

void doublyFree(doublyLinkedList* list) {
	while (!doublyIsEmpty(list)) {
		doublyDeleteLast(list);
	}
	if (list != NULL) {
		free(list);
	}
}
