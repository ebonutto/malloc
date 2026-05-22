#include "malloc.h"

int main()
{
	void *ptr;

	ptr = malloc(20);
	(void)ptr;
	ptr = malloc(40);
	(void)ptr;
	ptr = malloc(60);
	(void)ptr;
	show_alloc_mem_ex();
	return (0);
}
