#include "malloc.h"

#include <stdio.h> // printf()

int main()
{
	void *ptr;

	ptr = malloc(20);

	printf("Before\n");
	show_alloc_mem();

	free(ptr);

	printf("After\n");
	show_alloc_mem();

	return (0);
}
