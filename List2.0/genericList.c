#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)




/*
typedef uint32_t TickType_t;

typedef long BaseType_t;

typedef unsigned long UBaseType_t;

typedef uint32_t StackType_t;

#define configMAX_TASK_NAME_LEN			( 16 )


//链表项
struct xLIST_ITEM
{
    TickType_t xItemValue;          // 排序键值
    struct xLIST_ITEM * pxNext;     // 下一个节点
    struct xLIST_ITEM * pxPrevious; // 上一个节点
    void * pvOwner;                 // 所属对象（一般是任务TCB）
    struct xLIST * pxContainer;     // 当前所在的链表
};

typedef struct xLIST_ITEM ListItem_t;
typedef struct xLIST_ITEM MiniListItem_t;

//链表头
typedef struct xLIST
{
    UBaseType_t uxNumberOfItems;    // 链表节点总数
    ListItem_t * pxIndex;           // 遍历游标
    MiniListItem_t xListEnd;        // 尾哨兵节点
} List_t;


typedef struct tskTaskControlBlock
{
    volatile StackType_t * pxTopOfStack; // 栈顶指针

    ListItem_t xStateListItem;           // 状态链表项
    ListItem_t xEventListItem;           // 事件链表项
    UBaseType_t uxPriority;              // 任务优先级
    StackType_t * pxStack;               // 栈内存起始地址
    char pcTaskName[ configMAX_TASK_NAME_LEN ]; // 任务名
} tskTCB;
*/


typedef struct Node
{
	uint32_t value;
	struct Node *next;
	struct Node *prev;
	void *pcontainer;    //侵入节点所属容器，用于记录容器首地址。linux用节点成员和零地址结构体计算偏移地址来反推容器地址
	struct List *plist;  //记录节点所属链表delayList，readylist，suspendlist

}Node_t;


typedef struct List
{
	long len;
	Node_t *index;
	Node_t listEnd;
}List_t;

typedef struct TaskTCB
{
	long priority;
	char pcTaskName[16];
	Node_t node;
}TaskTCB_t;



void listInit(List_t *list)
{

	list->listEnd.pcontainer = NULL;
	list->listEnd.plist = NULL;
	list->listEnd.next = &list->listEnd;
	list->listEnd.prev = &list->listEnd;
	list->listEnd.value = 0xFFFFFFFFU;


	list->index= &list->listEnd;
	list->len = 0;
}


void nodeInit(Node_t *node, TaskTCB_t *tcb)
{
	node->pcontainer = tcb;
	node->plist = NULL;
	node->next = NULL;
	node->prev = NULL;

}


void listInsertEnd(List_t *list, Node_t *newNode)
{
	if(newNode->plist != NULL)
	{
		return;
	}

	Node_t *index = list->index;

	index->prev->next = newNode;
	newNode->prev = index->prev;

	newNode->next = index;
	index->prev = newNode;

	newNode->plist = list;
	list->len++;

}


void listInsertSort(List_t *list, Node_t *newNode)
{
	if(newNode->plist != NULL)
	{
		return;
	}


	uint32_t insertValue = newNode->value;
	Node_t *current = &list->listEnd;
    
	while(current->next->value <= insertValue)
	{
		current = current->next;
	}

	newNode->next = current->next;
	current->next->prev = newNode;

	current->next = newNode;
	newNode->prev = current;
	

	newNode->plist = list;
	list->len++;

}

void listRemove(Node_t *node)
{
	if(node->plist == NULL)
	{
		return;
	}

	node->prev->next = node->next;
	node->next->prev = node->prev;

	List_t *list = (List_t *)node->plist;

	if(list->index == node)
	{
		list->index = node->prev;
	}

	node->plist = NULL;
	list->len--;

}

//listEnd（哨兵） <--> A <--> B <--> C <--> listEnd（哨兵）
void listPrint(List_t *list)
{

	Node_t *current = (Node_t *)list->listEnd.next;

	while(current != &list->listEnd)
	{
		TaskTCB_t *tcb = (TaskTCB_t *)current->pcontainer;
		printf("任务: %s, 优先级: %ld, value: %u\n",tcb->pcTaskName,tcb->priority,current->value);
		current = current->next;
	}
}











int main(void) {

    // List_t readyList[5];
	// listInit(&readyList[0]);
	// listInit(&readyList[1]);

	// TaskTCB_t task1,task2,task3;
	// TaskTCB_t task4,task5,task6;

	// strcpy(task1.pcTaskName,"task1"); task1.priority = 0; nodeInit(&task1.node,&task1);
	// strcpy(task2.pcTaskName,"task2"); task2.priority = 0; nodeInit(&task2.node,&task2);
	// strcpy(task3.pcTaskName,"task3"); task3.priority = 0; nodeInit(&task3.node,&task3);


	// task1.node.value = task1.priority;
	// task2.node.value = task2.priority;
	// task3.node.value = task3.priority;

	// listInsertEnd(&readyList[0],&task1.node);
	// listInsertEnd(&readyList[0],&task2.node);
	// listInsertEnd(&readyList[0],&task3.node);


	// listPrint(&readyList[0]);


	// strcpy(task4.pcTaskName,"task4"); task4.priority = 1; nodeInit(&task4.node,&task4);
	// strcpy(task5.pcTaskName,"task5"); task5.priority = 1; nodeInit(&task5.node,&task5);
	// strcpy(task6.pcTaskName,"task6"); task6.priority = 1; nodeInit(&task6.node,&task6);

	// task4.node.value = task4.priority;
	// task5.node.value = task5.priority;
	// task6.node.value = task6.priority;

	// listInsertEnd(&readyList[1],&task4.node);
	// listInsertEnd(&readyList[1],&task5.node);
	// listInsertEnd(&readyList[1],&task6.node);

	// listPrint(&readyList[1]);




	// List_t delayList;

	// listInit(&delayList);

	// TaskTCB_t task7,task8,task9;


	// strcpy(task7.pcTaskName,"task7"); task7.priority = 2; nodeInit(&task7.node,&task7);
	// strcpy(task8.pcTaskName,"task8"); task8.priority = 2; nodeInit(&task8.node,&task8);
	// strcpy(task9.pcTaskName,"task9"); task9.priority = 2; nodeInit(&task9.node,&task9);


	// task7.node.value = 500;
	// task8.node.value = 300;
	// task9.node.value = 100;


	// listInsertSort(&delayList, &task8.node);
	// listInsertSort(&delayList, &task7.node);
	// listInsertSort(&delayList, &task9.node);





	// listPrint(&delayList);


	// listRemove(&task9.node);

	// listPrint(&delayList);




    	unsigned int i = 0;
    	unsigned char data[11] = { '1','2','3','4','5','6','7','8','9','0' };
     
    	memcpy(&data[2], data, 10 / 2);
		//data[10] = '\0';


		printf("%s",data);
     
    	return 0;
 







}