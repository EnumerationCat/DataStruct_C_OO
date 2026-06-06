#ifndef LINKEDLIST_H
#define LINKEDLIST_H


typedef struct Node {
	int data;
	struct Node *next;
	struct Node *prve;
} Node;


typedef struct List {
	Node *head;
	Node *tail;
	size_t size; //链表的长度
}List;


void listInit(List *list);
void listFree(List *list);

Node *getCurrentNode(List *list,size_t index);
size_t listGetLength(List *list);
int listGetData(List *list,size_t index);
void listSetData(List *list,size_t index,int data);

void listAdd(List *list, int data);
void listInsert(List *list, size_t index, int data);
void listDataDelete(List *list, int data);
void listIndexDelete(List *list,size_t index);

void printList(List *list);

void listTest();



#endif // LINKEDLIST_H