#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node {
	int	row,col,newVal,prevVal;
	struct	node *next;
} node;

typedef struct linkedList {
	int	size;
	node *head;
	node *tail;
} linkedList;

typedef struct doublyNode {
	linkedList *move;
	struct doublyNode *next;
	struct doublyNode *prev;

} doublyNode;

typedef struct doublyLinkedList {
	int	size;
	doublyNode *head;
	doublyNode *tail;

} doublyLinkedList;

node* initializeNode(int row, int col, int val, int prevVal);

linkedList* initializeLinkedList();

int isEmpty(linkedList*);

void insertLast(linkedList*,  int row ,  int col , int val, int prevVal);

void deleteFirst(linkedList*);

void freeList(linkedList* list);

doublyNode* initializeDoublyNode(linkedList *data);

doublyLinkedList* initializeDoublyLinkedList();

int doublyIsEmpty(doublyLinkedList*);

void doublyInsertLast(doublyLinkedList*, linkedList*);

void doublyDeleteAllAfter(doublyLinkedList* list, doublyNode *curMove);

doublyNode* doublyGetLast(doublyLinkedList* list);

void doublyDeleteLast(doublyLinkedList *list);

void doublyFree(doublyLinkedList*);

#endif