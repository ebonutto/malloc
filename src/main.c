#include "malloc.h"
#include "malloc_int.h"

#include <stdio.h> // printf()

void test_tiny(void);
void test_small(void);

int main()
{
	test_small();
	return (0);
}

void test_tiny(void)
{
	void *p1;
	void *p2;
	void *p3;
	void *p4;

	p1 = malloc(1);
	p2 = malloc(16);
	p3 = malloc(42);
	p4 = malloc(128);

	show_alloc_mem_ex();

	free(p1);
	free(p3);

	//show_alloc_mem_ex();

	free(p4);

	//show_alloc_mem_ex();

	free(p2);

	//show_alloc_mem_ex();
}

void test_small(void)
{
	void *p1;
	void *p2;
	void *p3;
	void *p4;

	p1 = malloc(129);
	p2 = malloc(200);
	p3 = malloc(512);
	p4 = malloc(1024);

	show_alloc_mem_ex();

	free(p1);
	free(p3);

	//show_alloc_mem_ex();

	free(p4);

	//show_alloc_mem_ex();

	free(p2);

	// show_alloc_mem_ex();
}
