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

/*
* initializes node and it's variables.
*/
node* initializeNode(int row, int col, int val, int prevVal);

/*
* initializes linkedList and return it.
*/
linkedList* initializeLinkedList();

/*
* if the linked list is empty-returns 1, otherwise -returns 0.
*/
int isEmpty(linkedList*);

/*
* inserts a node to the end of the linked list.
*/
void insertLast(linkedList*,  int row ,  int col , int val, int prevVal);

/*
* deletes the first node of the linked list .
*/
void deleteFirst(linkedList*);

/*
* frees memory resources allocated for the linked list.
*/
void freeList(linkedList* list);

/*
* initializes a doubly node for the doubly linked list.
*/
doublyNode* initializeDoublyNode(linkedList *data);

/*
* initializes a doubly linked list.
*/
doublyLinkedList* initializeDoublyLinkedList();

/*
* if the doubly linked list is empty-returns 1, otherwise -returns 0.
*/
int doublyIsEmpty(doublyLinkedList*);

/*
* inserts a node to the end of the doubly linked list.
*/
void doublyInsertLast(doublyLinkedList*, linkedList*);

/*
* deletes all nodes after the curMove pointer.
*/
void doublyDeleteAllAfter(doublyLinkedList* list, doublyNode *curMove);

/*
* gets the last node in the doubly linked list.
*/
doublyNode* doublyGetLast(doublyLinkedList* list);

/*
* deletes the last node of the doubly linked list.
*/
void doublyDeleteLast(doublyLinkedList *list);

/*
* frees memory resources allocated for the doubly linked list.
*/
void doublyFree(doublyLinkedList*);

#endif