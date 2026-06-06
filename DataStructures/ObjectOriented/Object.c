#include "Object.h"
#include "stdio.h"
#include "string.h"

/*********************************************************
 * Obeject Oriented Programming 面向对象编程
 * SOLID 五大面向对象设计原则（C语言模拟OOP适用）
 * 
 * 1. 单一职责原则 SRP：一个模块/结构体只负责一项功能
 * 2. 开闭原则 OCP：对扩展开放，对修改关闭
 * 3. 里氏替换原则 LSP：子类可无缝替换父类,但不能改变父类的功能
 * 4. 接口隔离原则 ISP：依赖最小接口，不依赖无用方法
 * 5. 依赖倒置原则 DIP：依赖抽象，不依赖具体实现
 *********************************************************/





/********************封装之继承****************************/

static void setWidth (struct Shape *s, int width)
{
	s->width = width;
}


static void setHeight(struct Shape *s, int height)
{
	s->height = height;
}




static int getArea(struct Shape *s)
{
	return (s->height * s->width);
}



//创建一个实列
static Rectangle rectAngle = {
    {   // struct Shape s
        .setWidth = setWidth,   // C++ 不支持 .setWidth，需改为顺序或去掉指定符
        .setHeight = setHeight
    },
    .getArea = getArea
};



void rectInit(struct Rectangle *rect)
{
	memcpy(rect, &rectAngle, sizeof(Rectangle));
	
}



/******************封装之多态*********************/


static void getName(struct Person *p, char *name)
{
	strcpy(name, p->name);
}


//父类转换成子类
static void childGetName(struct Person *p, char *name) {
    // p 实际指向某个子类对象中的 Person 子对象（且是第一个成员）
    // 强制转换为 Wrapper* 是安全的，因为内存布局完全一致
    struct Wrapper *child = (struct Wrapper *)p;
    child->getName(p, name);
}


static void getAge(struct Person *p, int *age)
{
	*age = p->age;
}



// 学生专用：添加 "-student"
static void studentGetName(struct Person *p, char *name) {
    sprintf(name, "%s-student", p->name);


}

// 老师专用：添加 "-teacher"
static void teacherGetName(struct Person *p, char *name) {
    sprintf(name, "%s-teacher", p->name);
}



static struct Person person = {
    "Yun",     // .name
    18,        // .age
    getName,   // .getName
    getAge     // .getAge
};

// Student 模板：先初始化 P（基类），再初始化自身的 getName
static Student student = {
    {   // struct Person P 的初始化
        "LiHua",           // .name
        20,                   // .age
        childGetName,      // .getName (基类指针指向转发函数)
        getAge         // .getAge
    },
    studentGetName           // Student::getName 指向真正实现
};

// Teacher 模板
static Teacher teacher = {
    {   // struct Person P
        "Zhang",
        35,
        childGetName,
        getAge
    },
    teacherGetName
};





void personInit(struct Person *p)
{
	memcpy(p,&person,sizeof(Person));

}


void studentInit(struct Student *s)
{
	memcpy(s,&student,sizeof(Student));

}

void teacherInit(struct Teacher *t)
{
	memcpy(t,&teacher,sizeof(Teacher));

}



/**********************里氏替换原则********************************/

// ========== Animal 方法实现 ==========
static void setRunSpeed(struct Animal *a, int s)
{
    a->runSpeed = s;
}

static int getRunSpeed(struct Animal *a)
{
    return a->runSpeed;
}

static int calculateRunTime(struct Animal *a, int distance)
{

    return distance / a->runSpeed;
}

// ========== FlyingBird 方法实现 ==========
static void setFlySpeed(struct FlyingBird *fb, int s)
{
    fb->flySpeed = s;
}

static int getFlySpeed(struct FlyingBird *fb)
{
    return fb->flySpeed;
}

static int calculateFlyTime(struct FlyingBird *fb, int distance)
{

    return distance / fb->flySpeed;
}


struct Hummingbird hummingbird = {
	{
		{
			0,
			setRunSpeed,
			getRunSpeed,
			calculateRunTime
		},
		0,
		setFlySpeed,
		getFlySpeed,
		calculateFlyTime

	}
    
};


struct Ostrich ostrich = {
	{
		{
			0,
			setRunSpeed,
			getRunSpeed,
			calculateRunTime
    	}

	}

};

// 初始化函数
void hummingbirdInit(struct Hummingbird *h)
{
	memcpy(h,&hummingbird,sizeof(Hummingbird));
}

void ostrichInit(struct Ostrich *o)
{
	memcpy(o,&ostrich,sizeof(Ostrich));
}
















