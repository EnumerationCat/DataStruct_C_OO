#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

/**
 * @file Stack.c
 * @brief 栈的实现文件，实现头文件声明的所有接口函数
 * @details 基于动态数组的栈核心逻辑实现，包含内存管理、入栈出栈、扩容等核心逻辑
 */

/**
 * @brief 初始化栈
 * @param stack 指向栈结构体的指针
 * @param initalCapacity 栈的初始容量
 * @details 为栈的动态数组分配初始内存，初始化栈顶、长度、容量等成员
 * @warning stack指针不可为NULL，否则会导致内存操作异常
 */
void stackInit(Stack *stack, size_t initalCapacity)
{
    // 为动态数组分配初始内存
    stack->data = (Type_t *)malloc(initalCapacity * sizeof(Type_t));
    stack->size = 0;    // 初始有效元素个数为0
    stack->top = -1;    // 栈顶初始化为-1（空栈）
    stack->capacity = initalCapacity; // 初始化容量
}

/**
 * @brief 调整栈的容量
 * @param stack 指向栈结构体的指针
 * @param newCapacity 新的栈容量
 * @details 调用realloc重新分配动态数组内存，更新栈的容量成员
 * @warning newCapacity建议不小于当前栈的有效元素个数，否则可能丢失数据
 */
void stackResize(Stack *stack, size_t newCapacity)
{
    // 重新分配内存，保留原有数据
    stack->data = (Type_t *)realloc(stack->data, newCapacity * sizeof(Type_t));
    stack->capacity = newCapacity; // 更新容量
}

/**
 * @brief 获取栈的有效元素个数
 * @param stack 指向栈结构体的指针
 * @return 栈的实际长度（size成员）
 * @note 时间复杂度O(1)，直接返回成员值
 */
size_t stackGetLength(Stack *stack)
{
    return stack->size;
}

/**
 * @brief 入栈操作
 * @param stack 指向栈结构体的指针
 * @param data 要入栈的数据
 * @details 1. 检查栈容量，若栈顶已到容量上限则扩容2倍；2. 栈顶top加1，将数据存入栈顶位置；3. 有效元素个数size加1
 * @note 扩容策略为翻倍，可根据需求调整（如固定增量）
 */
void stackPush(Stack *stack, Type_t data)
{
    // 容量不足时扩容（栈顶下标等于容量-1表示满栈）
    if (stack->top == stack->capacity - 1)
    {
        stackResize(stack, stack->capacity * 2);
    }

    // 栈顶上移，存入数据，有效个数+1
    stack->data[++stack->top] = data;
    stack->size++;
}

/**
 * @brief 出栈操作
 * @param stack 指向栈结构体的指针
 * @return 栈顶元素值；若栈空则打印错误信息并返回-1
 * @details 1. 检查栈是否为空；2. 有效元素个数size减1；3. 返回栈顶元素，栈顶top减1
 * @warning 栈空时出栈会返回-1，若Type_t类型非int需调整返回值逻辑
 */
Type_t stackPop(Stack *stack)
{
    // 栈空判断
    if (stack->top == -1)
    {
        printf("Stack is empty\n");
        return -1;
    }

    stack->size--; // 有效个数减1
    // 返回栈顶元素，栈顶下移
    return stack->data[stack->top--];
}

/**
 * @brief 打印栈的所有元素及长度
 * @param stack 指向栈结构体的指针
 * @details 1. 检查栈是否为空；2. 从栈底到栈顶遍历打印所有元素；3. 打印栈的有效长度
 * @note 打印顺序为“栈底 -> 栈顶”，符合栈的遍历逻辑
 */
void printStack(Stack *stack)
{
    // 栈空提示
    if (stack->top == -1)
    {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack: ");
    // 遍历从栈底（下标0）到栈顶（下标top）的所有元素
    for (size_t i = 0; i <= stack->top; i++)
    {
        printf("%d ", stack->data[i]);
    }
    // 打印栈的有效长度
    printf("Length:%zu\n", stackGetLength(stack));
}

/**
 * @brief 释放栈的动态内存
 * @param stack 指向栈结构体的指针
 * @details 1. 释放动态数组的内存；2. 将所有成员置空/置零，避免野指针和内存泄漏
 * @note 释放后栈结构体本身并未释放（若为堆分配需外部手动释放）
 */
void stackFree(Stack *stack)
{
    free(stack->data);   // 释放动态数组内存
    stack->data = NULL;  // 置空避免野指针
    stack->size = 0;     // 重置长度
    stack->top = -1;     // 重置栈顶
    stack->capacity = 0; // 重置容量
}

/**
 * @brief 栈的测试函数
 * @details 测试流程：
 * 1. 初始化容量为4的栈；
 * 2. 循环输入数据入栈（输入-1结束）；
 * 3. 打印入栈后的栈；
 * 4. 执行3次出栈操作并打印结果；
 * 5. 打印出栈后的栈；
 * 6. 释放栈内存；
 * 7. 打印释放后的栈（验证空栈）
 */
void stackTest(void)
{
    Stack stack; // 栈结构体（栈上分配）
    int mydata;  // 临时存储输入数据

    // 初始化栈，初始容量4
    stackInit(&stack, 4);

    // 循环输入数据，-1结束入栈
    do
    {
        scanf("%d", &mydata);
        if (mydata != -1)
            stackPush(&stack, mydata);
    } while (mydata != -1);

    // 打印入栈后的栈
    printStack(&stack);

    // 执行3次出栈并打印结果
    printf("Stack pop: %d\n", stackPop(&stack));
    printf("Stack pop: %d\n", stackPop(&stack));
    printf("Stack pop: %d\n", stackPop(&stack));

    // 打印出栈后的栈
    printStack(&stack);

    // 释放栈内存
    stackFree(&stack);
    // 验证释放后的栈状态
    printStack(&stack);
}