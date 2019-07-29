#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>

node* initializeNode(int row,  int col,  int newVal,  int prevVal) {
	node *newNode = (node*)malloc(sizeof(node));
	if (newNode == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
	newNode->row = row;
	newNode->col = col;
	newNode->newVal = newVal;
	newNode->prevVal = prevVal;
	newNode->next = NULL;
	return newNode;
}

linkedList* initializeLinkedList() {
	linkedList *newNode = (linkedList*)calloc(1, sizeof(linkedList));
	if (newNode == NULL) {
		printf("Error: calloc failed\n");
		exit(0);
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
	node *newNode = initializeNode(row, col, Newval, prevVal);
	node *head = list->head;
	node *tail = list->tail;
	node *lastNode;
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
	node *head = list->head;
	node *next;

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

void clear(linkedList* list) {
	while (!singly_isEmpty(list)) {
		singly_removeFirst(list);
	}
	free(list);
}

doublyNode* initializeDoublyNode(linkedList *move) {
	doublyNode *newNode = (doublyNode*)calloc(1, sizeof(doublyNode));
	if (newNode == NULL) {
		printf("Error: calloc failed\n");
		exit(0);
	}
	newNode->move = move;
	return newNode;
}

doublyLinkedList* initializeDoublyLinkedList() {
	doublyLinkedList *newNode = (doublyLinkedList*)calloc(1, sizeof(doublyLinkedList));
	if (newNode == NULL) {
		printf("Error: calloc failed\n");
		exit(0);
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
	doublyNode *newNode = doubly_createNewNode(move);
	doublyNode *head = list->head;
	doublyNode *tail = list->tail;
	doublyNode *prevNode;
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

void doublyDeleteAfter(doublyLinkedList* list, doublyNode *node) {
	doublyNode *tmp = doublyGetLastNode(list);
	if (tmp != NULL) {
		while (tmp != node) {
			doublyDeleteLast(list);
			tmp = doublyGetLastNode(list);
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
	doublyNode *last = doublyGetLastNode(list);
	doublyNode *prev;

	if (!doublyIsEmpty(list)) {
		prev = last->prev;
		list->tail = prev;
		if (prev != NULL) { 
			prev->next = NULL;
		}
		singly_clear(last->move);
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

void doublyClear(doublyLinkedList* list) {
	while (!doublyIsEmpty(list)) {
		doublyDeleteLast(list);
	}
	if (list != NULL) {
		free(list);
	}
}
