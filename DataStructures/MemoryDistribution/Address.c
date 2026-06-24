#include "Address.h"
#include "stdio.h"
#include "stdlib.h"

int global_inited = 10;
int global_uninited;





void addr_Print(void)
{
	int local_var = 20;
	const char *str = "hello";
	static int static_var = 30;
	int* heap_var = (int *)malloc(sizeof(int));

	printf("addr of code(txt) : %p\n", addr_Print);
	printf("addr of string(rdo) : %p\n", str);

	printf("addr of global_inited(data) : %p\n", (void *)&global_inited);
	printf("addr of globalstatic(data) : %p\n", (void *)&static_var);
	printf("addr of global_uninited(bbs) : %p\n", (void *)&global_uninited);

	printf("addr of local(stack) : %p\n", (void *)&local_var);
	printf("addr of heap(heap): %p\n", (void *)heap_var);






    free(heap_var);







}
