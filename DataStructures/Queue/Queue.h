#ifndef QUEUE_H
#define QUEUE_H



typedef int Type_t;

typedef struct Queue
{
	Type_t *data;

	size_t size;
	size_t capacity;
	size_t front;  //要出队列的位置
	size_t rear;  //要添加的空位置

}Queue;


void queueInit(Queue *queue, size_t  initalCapacity);
void queueFree(Queue *queue);

size_t queueGetLength(Queue *queue);

void enQueue(Queue *queue, Type_t Data);
Type_t deQueue(Queue *queue);


void printQueue(Queue *queue);

void queueTest(void);


#endif // QUEUE_H
