#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"


node* createNewNode( int row,  int col,  int val, int prevVal) {
	node *newNode = (node*)calloc(1,sizeof(node));
	if (newNode == NULL) {
		printf("Error: calloc failed\n");
		exit(1);
	}
	newNode->row = row;
	newNode->col= col;
	newNode->val = val;
	newNode->prevVal = prevVal;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

linkedList* createNewLinkedList() {
	linkedList *newNode = (linkedList*)calloc(1, sizeof(linkedList));
	if (newNode == NULL) {
		printf("Error: calloc failed\n");
		exit(1);
	}
	return newNode;
}
int isEmpty(linkedList *list) {
	if (list == NULL) {
		return 1;
	}
	if (list->size > 0) {
		return 0;
	}
	return 1;
}

void addLast(linkedList *list, int row,  int col,  int val,  int prevVal) {
	node *newNode = createNewNode(row, col, val, prevVal);
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
		newNode->prev = lastNode;
		list->tail = newNode;
	}
	list->size++;
	return;
}

node* deleteFirst(linkedList* list) {
	node *prevHead;
	node *head = list->head;
	node *next;
	int col;
	int row;
	int val;
	int prevVal;

	if (isEmpty(list)) {
		return NULL;
	}
	next = head->next;
	prevHead->col = list->head->col;
	prevHead->row = list->head->row;
	prevHead->val = list->head->val;
	prevHead->prevVal = list->head->prevVal;
	list->head = next;
	free(head);
	list->size--;	
	if (list->size <= 1) { 
		list->tail = NULL;
	}
	return prevHead;
}

void removeLast(linkedList *list) {
	node *last = getLastNode(list);
	node *prevNode;

	if (isEmpty(list)) {
		return;
	}
	prevNode = last->prev;
	list->tail = prevNode;
	if (prevNode != NULL) {
		prevNode->next = NULL;
	}
	free(last);
	list->size--;	
	if (list->size <= 1) {
		list->tail = NULL;
	}
	if (list->size == 0) { 
		list->head = NULL;
	}
	return;
}


void clear(linkedList* list) {
	while (!isEmpty(list)) {
		removeLast(list);

	}
	if (list != NULL) {
		free(list);
	}
}

node* getLastNode(linkedList* list) {
	if (isEmpty(list)) { 
		return NULL;
	}
	if (list->tail == NULL) { 
		return list->head;
	}
	return list->tail; 
}

void removeAfter(linkedList* list, node *thisNode) {
	node* tmp = getLastNode(list);
	if (tmp != NULL) {
		while (tmp != thisNode) {
			removeLast(list);
			tmp = getLastNode(list);
		}
	}
}