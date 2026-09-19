#include "Linux_Container_of.h"

//linux container_of 宏的思想
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})


void find_struct(int *member)
{
    unsigned long offset = 0;
    struct abc* p = NULL;

    offset = (uintptr_t)&((struct abc*)0)->b;
    printf(" member offset: %ld \n", offset);

    p = (struct abc *)((char*)member - offset);

    printf(" a:%d, b:%d c: %d \n", p->a, p->b, p->c);
}


void find_struct_test(void)
{

	struct abc data = 
	{
		1,
		2,
		3
	};

	find_struct(&data.b);

}


