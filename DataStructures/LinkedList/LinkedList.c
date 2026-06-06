#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"



void listInit(List *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}


size_t listGetLength(List *list)
{
	return list->size;
}

int listGetData(List *list,size_t index)
{
	Node *current = getCurrentNode(list,index);
	return current->data;
}


Node *getCurrentNode(List *list,size_t index)
{

	if(index<0||index>list->size){
		printf("Error: Index %zu out of bounds (size: %zu)\n", index, list->size);
		return NULL;
	}
	
	Node* previous = NULL;
	Node* current = NULL;
	int currentIndex=0;
	for(current = list->head; current != NULL; previous = current, current = current->next){
		if(currentIndex == index){

			current->prve = previous;
			return current;
		}
		currentIndex++;
	}
	//消除编译警告
	return NULL;

}

void listSetData(List *list,size_t index,int data)
{
	if(index<0||index>=list->size){
		printf("Error: Index %zu out of Data bounds\n", index);
		return;
	}
	Node *current = getCurrentNode(list,index);
	current->data = data;
}


//依次顺序插入
void listAdd(List *list, int data){
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->next = NULL;

	if(list->head == NULL){
		list->head = newNode;
		list->tail = newNode;
	}else{
		list->tail->next = newNode;
		list->tail = newNode;
	}
	list->size++;
}

void listInsert(List *list, size_t index, int data)
{

	if(index<0||index>list->size){
		printf("Error: Index %zu out of bounds (size: %zu)\n", index, list->size);
		return;
	}

	Node* previous = NULL;
	Node* current = NULL;
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	
	// 空链表情况直接插入第一个节点
	if(list->head == NULL){
		listAdd(list,data);
		return;
	}

	//最前面插入新节点
	if(index == 0){
		newNode->next = list->head;
		list->head =newNode;

	}else{
		//向中间位置插入新节点，遍历前一项指针指向新插入节点，新插入节点指向当前节点
		int currentIndex=0;
		for(current = list->head; current != NULL; previous = current, current = current->next){
			
			if(currentIndex==index){
				previous->next = newNode;
				newNode->next = current;
				break;
			}
			currentIndex++;
		}
		//如果是末尾插入，需更新尾节点
		if(current==NULL){
			previous->next = newNode;
			newNode->next = NULL;
			list->tail = newNode;
		}

	}
	list->size++;
}




void listDataDelete(List *list, int data){

	Node* previous = NULL;
	Node* current = NULL;
	
	if(list->head == NULL){
		printf("List is empty\n");
		return;
	}
	

	for(current = list->head; current != NULL; previous = current, current = current->next){
		if(current->data == data){

			
			if(previous != NULL){
				previous->next = current->next;

			}else{
				list->head = current->next;

			}
			free(current);
			list->size--;
			break;
		}
	}
}

void listIndexDelete(List *list,size_t index){
	
	if(index<0||index>list->size-1){
		printf("Error: Index %zu out of delete bounds\n", index);
		return;
	}

	Node* previous = NULL;
	Node* current = NULL;
	int currentIndex=0;

	for(current = list->head; current != NULL; previous = current, current = current->next){
		if(currentIndex == index){

			
			if(previous != NULL){
				previous->next = current->next;

			}else{
				list->head = current->next;

			}
			free(current);
			list->size--;
			break;
		}
		currentIndex++;
	}

}


void listFree(List *list){
	Node* current = NULL;
	Node* next = NULL;

	if(list->head == NULL){
		printf("List is empty\n");
		return;
	}

	for(current = list->head; current != NULL; current = next){
		next = current->next;
		free(current);
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

}





void printList(List *list){

	Node* current = NULL;

	if(list->head == NULL){
		printf("List is empty\n");
		return;
	}

	for(current = list->head; current != NULL; current = current->next){
		printf("%d->", current->data);
	}
	printf("NULL ");


	printf("Length:%zu\n",listGetLength(list));


}



void listTest(){
	List list;
	int mydata;
	listInit(&list);
	do{
		scanf("%d", &mydata);
		if(mydata != -1)
			listAdd(&list, mydata);
	} while(mydata != -1);
	printList(&list);

	do{
		scanf("%d", &mydata);
		if(mydata != -1)
			listInsert(&list,5,mydata);
	} while(mydata != -1);
	printList(&list);

	printf("Data:%d\n",listGetData(&list,0));

	Node *curr = getCurrentNode(&list,1);

	if(curr!=list.head)
	{
		printf("PreData:%d Data:%d\n",curr->prve->data,curr->data);

	}else{
		printf("PreData:NULL Data:%d\n",curr->data);
	}
	




	// listIndexDelete(&list, 0);
	// printList(&list);
	// listIndexDelete(&list, 3);
	// printList(&list);

	// listSetData(&list,3,666);
	// printList(&list);

	listFree(&list);
	printList(&list);
}

