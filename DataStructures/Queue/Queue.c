
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"





void queueInit(Queue *queue, size_t  initalCapacity){

	queue->data = (Type_t*)malloc(initalCapacity*sizeof(Type_t));

	queue->size = 0;
	queue->capacity = initalCapacity;
	queue->front = 0;
	queue->rear = 0;

}


void enQueue(Queue *queue, Type_t Data){
	if(queue->size == queue->capacity)
	{
		printf("Queue is full! Failed to enqueue Data: %d\n", Data);
		return;
	}

	//队尾添加元素
	queue->data[queue->rear] = Data;

	queue->rear = (queue->rear+1)%queue->capacity;

	queue->size++;



}


Type_t deQueue(Queue *queue)
{
	if(queue->size == 0)
	{
		printf("Queue is empty! Failed to dequeue\n");
		return -1;
	}

	Type_t Data = queue->data[queue->front];
	//队头递增
	queue->front = (queue->front+1)%queue->capacity;

	//长度递减
	queue->size--;

	return Data;

}



size_t queueGetLength(Queue *queue){
	return queue->size;
}



void queueFree(Queue *queue){
	free(queue->data);
	queue->data = NULL;
	queue->size = 0;
	queue->capacity = 0;
	queue->front = 0;
	queue->rear = 0;
}



void printQueue(Queue *queue)
{
	printf("Queue: ");
	for(size_t i = 0;i<queue->size;i++)
	{
		size_t index = (queue->front+i)%queue->capacity;
		printf("%d ",queue->data[index]);
	}
	printf("Length:%zu\n",queueGetLength(queue));

}



void queueTest(void)
{
	Queue queue;

	int mydata;  // 临时存储输入数据

    queueInit(&queue,5);
    // 循环输入数据，-1结束入栈
    do
    {
        scanf("%d", &mydata);
        if (mydata != -1)
           enQueue(&queue,mydata);
    } while (mydata != -1);

	printQueue(&queue);

	printf("deQueue:%d\n",deQueue(&queue));
	printf("deQueue:%d\n",deQueue(&queue));
	printf("deQueue:%d\n",deQueue(&queue));

	printQueue(&queue);


	queueFree(&queue);


}