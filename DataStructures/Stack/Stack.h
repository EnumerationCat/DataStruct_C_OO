#ifndef STACK_H
#define STACK_H

/**
 * @file Stack.h
 * @brief 栈的头文件，定义栈的结构体、数据类型及函数接口
 * @details 实现基于动态数组的栈结构，支持动态扩容、入栈、出栈、内存释放等核心操作
 */

/**
 * @brief 栈存储的数据类型定义
 * @note 可根据实际需求修改类型（如char、float等），当前为int类型
 */
typedef int Type_t;

/**
 * @brief 栈的结构体定义（动态数组实现）
 * @details 采用动态内存分配管理栈空间，支持容量自动扩容，通过top标识栈顶位置，size记录有效元素个数
 */
typedef struct Stack
{
    Type_t *data;      ///< 指向栈存储数据的动态数组首地址
    size_t size;       ///< 栈中有效元素的个数（栈的实际长度）
    size_t top;        ///< 栈顶下标（-1表示空栈，此处用size_t故初始值为-1需注意类型转换）
    size_t capacity;   ///< 栈的容量（动态数组可容纳的最大元素个数）
} Stack;

/**
 * @brief 初始化栈
 * @param stack 指向栈结构体的指针（需提前分配内存）
 * @param initalCapacity 栈的初始容量（建议设置合理值，减少扩容次数）
 * @note 初始化后栈顶top为-1，size为0，动态数组内存由malloc分配
 */
void stackInit(Stack *stack, size_t initalCapacity);

/**
 * @brief 释放栈占用的动态内存
 * @param stack 指向栈结构体的指针
 * @note 释放后会将栈的成员置空/置零，避免野指针和内存泄漏
 */
void stackFree(Stack *stack);

/**
 * @brief 调整栈的容量（扩容/缩容）
 * @param stack 指向栈结构体的指针
 * @param newCapacity 栈的新容量
 * @note 内部调用realloc重新分配内存，若扩容失败可能返回NULL（当前实现未做容错，可根据需求补充）
 */
void stackResize(Stack *stack, size_t newCapacity);

/**
 * @brief 获取栈中有效元素的个数
 * @param stack 指向栈结构体的指针
 * @return 栈的实际长度（size成员值）
 */
size_t stackGetLength(Stack *stack);

/**
 * @brief 入栈操作（向栈顶添加元素）
 * @param stack 指向栈结构体的指针
 * @param data 要入栈的数据
 * @note 若栈容量不足，会自动扩容为原容量的2倍
 */
void stackPush(Stack *stack, Type_t data);

/**
 * @brief 出栈操作（从栈顶移除并返回元素）
 * @param stack 指向栈结构体的指针
 * @return 栈顶元素值；若栈为空，打印错误信息并返回-1
 * @note 出栈后栈顶top减1，有效元素个数size减1
 */
Type_t stackPop(Stack *stack);

/**
 * @brief 打印栈中所有元素及栈的长度
 * @param stack 指向栈结构体的指针
 * @note 若栈为空，打印"Stack is empty"提示
 */
void printStack(Stack *stack);

/**
 * @brief 栈的测试函数
 * @details 演示栈的初始化、循环入栈（输入-1结束）、出栈、打印、内存释放的完整流程
 */
void stackTest(void);

#endif // STACK_H