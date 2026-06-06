#ifndef OBJECT_H
#define OBJECT_H

/********************C与继承*******************************/
//基类
typedef struct Shape 
{

	int width;
	int height;

	void (*setWidth)(struct Shape *s, int width);
	void (*setHeight)(struct Shape *s, int height);

}Shape;



//派生类

typedef struct Rectangle
{
	struct Shape s;

	int(*getArea)(struct Shape *s);

}Rectangle;


/*********************************************************/



/**********************C与多态*****************************/
typedef struct Person
{
	char name[20];
	int age;
	void(*getName)(struct Person *p, char *name);
	void(*getAge)(struct Person *p, int *age);
}Person;


// 定义一个与 Student/Teacher 布局相同的匿名结构体
typedef struct Wrapper {
    struct Person P;
    void (*getName)(struct Person *p, char *name);
}Wrapper;



typedef struct Student
{
	struct Person P;
	void(*getName)(struct Person *p, char *name);
}Student;


typedef struct Teacher
{
	struct Person P;
	void(*getName)(struct Person *p, char *name);
}Teacher;

/*************************里氏替换原则********************************/

/*******鸵鸟不会飞可以抽象Animal做一个飞行能力的属性，鸵鸟继承Animal*******/

// 抽象基类：动物（仅包含奔跑能力）
typedef struct Animal {
    int runSpeed;
    void (*setRunSpeed)(struct Animal *a, int s);
    int (*getRunSpeed)(struct Animal *a);
    int (*calculateRunTime)(struct Animal *a, int distance); // 奔跑耗时
} Animal;

// 有飞行能力的鸟类（继承Animal）
typedef struct FlyingBird {
    struct Animal animal;       // 继承Animal
    int flySpeed;
    void (*setFlySpeed)(struct FlyingBird *fb, int s);
    int (*getFlySpeed)(struct FlyingBird *fb);
    int (*calculateFlyTime)(struct FlyingBird *fb, int distance); // 飞行耗时
} FlyingBird;

// 无飞行能力的鸟类（继承Animal）
typedef struct NonFlyingBird {
    struct Animal animal;       // 继承Animal
    // 无飞行相关属性，仅保留奔跑能力
} NonFlyingBird;

// 派生类：蜂鸟（有飞行能力）
typedef struct Hummingbird {
    struct FlyingBird flyingbird;
} Hummingbird;

// 派生类：鸵鸟（无飞行能力）
typedef struct Ostrich {
    struct NonFlyingBird nonflyingbird;
} Ostrich;




void rectInit(struct Rectangle *rect);

void personInit(struct Person *p);
void studentInit(struct Student *s);
void teacherInit(struct Teacher *t);


void hummingbirdInit(struct Hummingbird *h);
void ostrichInit(struct Ostrich *o);


#endif // OBJECT
