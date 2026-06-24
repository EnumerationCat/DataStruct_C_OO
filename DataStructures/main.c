#include <stdio.h>
#include <stdlib.h>
#include "DynamicArray/DynamicArray.h"	
#include "LinkedList/LinkedList.h"
#include "Stack/Stack.h"
#include "Queue/Queue.h"
#include "ObjectOriented/Object.h"
#include "Module_Init/module_init.h"
#include "MemoryDistribution/Address.h"



#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)



int main() {


	//embedi_module_init();

	//dynamicArrayTest();
	//listTest();
	//stackTest();
	//queueTest();

	// /************************子类继承父类*********************************/
	// struct Rectangle myRect = {0};
	// rectInit(&myRect);

	// myRect.s.setHeight(&myRect.s,100);
	// myRect.s.setWidth(&myRect.s,100);
	// LOG_INFO("%d\n",myRect.getArea(&myRect.s));



	// /************************父类调用子类*********************************/
	// char Name[20];
	// int Age;
	
	// struct Person person = {0};
	// personInit(&person);
	// person.getName(&person, Name);
	// person.getAge(&person, &Age);
	// LOG_INFO("Name:%s Age:%d\n",Name,Age);


	// struct Student student  = {0};
	// studentInit(&student);
	// Person *person2 = (Person*)&student;
	// person2->getName(person2,Name);
	// person2->getAge(person2,&Age);
	// LOG_INFO("Name:%s Age:%d\n",Name,Age);


	// struct Teacher teacher = {0};
	// teacherInit(&teacher);
	// Person *person3 = (Person*)&teacher;
	// person3->getName(person3, Name);
	// person3->getAge(person3, &Age);
	// LOG_INFO("Name:%s Age:%d\n", Name, Age);

	// /***************************里氏替换原则********************************/

	// int flySpeed, runSpeed;
	// int flyTime, runTime;

	// struct Hummingbird hummingbird = {0};
	// hummingbirdInit(&hummingbird);
	// hummingbird.flyingbird.setFlySpeed(&hummingbird.flyingbird, 10);
	// hummingbird.flyingbird.animal.setRunSpeed(&hummingbird.flyingbird.animal, 5);
	// flyTime = hummingbird.flyingbird.calculateFlyTime(&hummingbird.flyingbird, 100);
	// runTime = hummingbird.flyingbird.animal.calculateRunTime(&hummingbird.flyingbird.animal, 100);

	// flySpeed = hummingbird.flyingbird.getFlySpeed(&hummingbird.flyingbird);
	// runSpeed = hummingbird.flyingbird.animal.getRunSpeed(&hummingbird.flyingbird.animal);
	// LOG_INFO("Hummingbird fly speed: %d m/s\n", flySpeed);
	// LOG_INFO("Hummingbird run speed: %d m/s\n", runSpeed);
	// LOG_INFO("Hummingbird fly time for 100m: %ds\n", flyTime);
	// LOG_INFO("Hummingbird run time for 100m: %ds\n", runTime);

	// struct Ostrich ostrich = {0};
	// ostrichInit(&ostrich);
	// ostrich.nonflyingbird.animal.setRunSpeed(&ostrich.nonflyingbird.animal, 8);
	// runTime = ostrich.nonflyingbird.animal.calculateRunTime(&ostrich.nonflyingbird.animal, 100);
	// runSpeed = ostrich.nonflyingbird.animal.getRunSpeed(&ostrich.nonflyingbird.animal);
	// LOG_INFO("Ostrich run speed: %d m/s\n", runSpeed);
	// LOG_INFO("Ostrich run time for 100m: %ds\n", runTime);


	addr_Print();


	
	
    return 0;
}