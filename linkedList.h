#ifndef LINKEDLIST_H

#define  LINKEDLIST_H
typedef struct node {
	int col;
	int row;
	int val;
	int prevVal;
	struct node *next;
	struct node *prev;
}node;


typedef struct linkedList {
	int size;
	node *head;
	node *tail;
	
} linkedList;

node* createNewNode(int row, int col, int val, int prevVal);

linkedList* createNewLinkedList();

int isEmpty(linkedList *list);

void addLast(linkedList *list, int row, int col, int val, int prevVal);

node* deleteFirst(linkedList* list);

void removeLast(linkedList *list);

void clear(linkedList* list);

node* getLastNode(linkedList* list);

void removeAfter(linkedList* list, node *node);

#endif 

