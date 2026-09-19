#ifndef LINUX_CONTAINER_OF_H
#define LINUX_CONTAINER_OF_H

#include <stdio.h>

struct abc {
    int a;
    int b;
    char c;
};


void find_struct(int *member);
void find_struct_test(void);


#endif