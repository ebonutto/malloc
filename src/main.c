#include "malloc.h"

#include <stdio.h> // printf()

int main()
{
	void *ptr;

	printf("t_zone size : %zu\n", sizeof(t_zone));

	printf("tiny size : %zu\n", TINY_SIZE);
	ptr = malloc(10);
	printf("Before\n");
	show_alloc_mem();
	printf("Before\n");
	free(ptr);

	printf("After\n");
	show_alloc_mem();

	return (0);
}
