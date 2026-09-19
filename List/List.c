#include <stdio.h>
#include <stdlib.h>


#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

typedef struct Node
{
	int data;
	struct Node *prev;
	struct Node *next;
}Node;


typedef struct List
{
	Node *head;
	Node *tail;
	size_t len;
}List;


void listInit(List *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
}


//顺序插入
void listAdd(List *list, int data)
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;


	if(NULL == list->head)
	{
		list->head = newNode;
		list->tail = newNode;
	}else{
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}


	list->head->prev = list->tail;
	list->tail->next = list->head;
	list->len++;
}




void listInsert(List *list, size_t index, int data)
{

	if(index<0||index>list->len){
		printf("Error: Index %zu out of bounds\n", index, list->len);
		return;
	}

	Node* current = NULL;
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;


	
	// 空链表情况直接插入第一个节点，使其指向自身
	if(list->head == NULL&&index == 0){

		list->head = newNode;
		list->tail = newNode;


	}

	//头插并更新头节点
	if(index == 0){
		newNode->next = list->head;
		list->head->prev = newNode;

		list->head =newNode;

	}else{
		//向中间位置插入新节点，遍历前一项指针指向新插入节点，新插入节点指向当前节点
		int currentIndex=0;
		current = list->head;
		do{
			if(currentIndex==index){
				current->prev->next = newNode;
				newNode->prev = current->prev;
				newNode->next = current;
				current->prev = newNode;
				break;
			}

			current = current->next;
			currentIndex++;

		}while(current != list->head);

		//末尾插入，需更新尾节点
		if(current == list->head){
			current->prev->next = newNode;
			newNode->prev = current->prev;

			list->tail = newNode;
		}

	}


	list->tail->next = list->head;
	list->head->prev = list->tail;
	list->len++;
}

size_t getListLen(List *list)
{
	return list->len;
}

void listInsertHead(List *list,int data)
{
	listInsert(list,0,data);
}


void listInsertTail(List *list, int data)
{
	listInsert(list,list->len,data);
}

Node *getCurrentNode(List *list, size_t index)
{

	if(index<0||index>=list->len)
	{
		printf("%zu is out of bounds\n",index);
		return NULL;
	}

	Node *current = NULL;
	size_t currentIndex = 0;
	current = list->head;
	do{

		if(currentIndex == index)
		{
			return current;
		}
		current = current->next;
		currentIndex++;
	}while(current != list->head);

	return NULL;
}




void printList(List *list){

	Node* current = NULL;

	if(list->head == NULL){
		printf("List is empty\n");
		return;
	}

    current = list->head;
    do {
        printf("%d", current->data);
        current = current->next;
        // 未回到头节点时才打印箭头，避免末尾多余符号
        if(current != list->head){
            printf("<->");
        }
    } while(current != list->head);


	printf(" len:%zu",getListLen(list));
    printf("\n");


	printf("head:%d<->tail:%d\n",list->tail->next->data,list->head->prev->data);


}



int main(void) {

	List list;
	int mydata;

	listInit(&list);


	do{
		scanf("%d", &mydata);
		if(mydata != -1)
			//listAdd(&list, mydata);
			listInsertHead(&list,mydata);
			//listInsertTail(&list,mydata);
	} while(mydata != -1);



	printList(&list);




	Node *current = getCurrentNode(&list,1);


	printf("%d\n",current->prev->data);
	printf("%d\n",current->data);
	printf("%d\n",current->next->data);





	



		

}