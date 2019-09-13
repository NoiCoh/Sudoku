#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*
* the struct represents a node of a linked list struct 
* which include all the information needed for a single user move in the board.
*/
typedef struct node {
	int	row,col,newVal,prevVal;
	struct	node *next;
} node;

/* 
* the struct represents a linked list
* redo, undo, reset function uses this struct for saving moves in the game.
*/
typedef struct linkedList {
	int	size;
	node *head;
	node *tail;
} linkedList;

/*
* the struct represents a doubly node of a doubly linked list struct
* which include all the information needed for a single user move in the board.
*/
typedef struct doublyNode {
	linkedList *move;
	struct doublyNode *next;
	struct doublyNode *prev;

} doublyNode;

/*
* the struct represents a doubly linked list
* redo, undo, reset function uses this struct for saving moves in the game.
*/
typedef struct doublyLinkedList {
	int	size;
	doublyNode *head;
	doublyNode *tail;
} doublyLinkedList;

/*
* initializes node and it's variables.
* return value: initialize node.
*/
node* initializeNode(int row, int col, int val, int prevVal);

/*
* initializes linkedList and return it.
*/
linkedList* initializeLinkedList();

/*
* The function checks if the linked list is empty.
* return value : if the linked list is empty-returns 1, otherwise - 0.
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
* return value : initialized doubly node
*/
doublyNode* initializeDoublyNode(linkedList *data);

/*
* initializes a doubly linked list and returns it.
*/
doublyLinkedList* initializeDoublyLinkedList();

/*
* The function checks if the linked list is empty.
* return value : if the doubly linked list is empty-returns 1, otherwise - 0.
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
* the function gets the last node in the doubly linked list and returns it.
*/
doublyNode* doublyGetLast(doublyLinkedList* list);

/*
* The function deletes the last node of the doubly linked list.
*/
void doublyDeleteLast(doublyLinkedList *list);

/*
* frees memory resources allocated for the doubly linked list.
*/
void doublyFree(doublyLinkedList*);

#endif