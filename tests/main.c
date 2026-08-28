#include "malloc.h"

#include <stddef.h> // NULL
#include <string.h> // memcmp(), memset()

#define TINY_MAX 128
#define SMALL_MAX 1024

static void test_show(void)
{
	void *a, *b;

	a = malloc(42);
	b = malloc(420);
	memset(a, 'A', 42);
	memset(b, 'B', 420);

	show_alloc_mem();
	show_alloc_mem_ex();

	free(a);
	free(b);

	show_alloc_mem();
	show_alloc_mem_ex();
}

static void test_basic(void)
{
	void *a, *b, *c;

	a = malloc(42);
	b = malloc(420);
	c = malloc(4200);

	show_alloc_mem_ex();

	free(c);
	free(b);
	free(a);

	show_alloc_mem();
}

static void test_zero(void)
{
	void *ptr;

	ptr = malloc(0);

	ptr = calloc(42, 0);
	ptr = calloc(0, 42);
	ptr = calloc(0, 0);

	ptr = realloc(NULL, 0);

	(void)ptr;

	free(NULL);

	show_alloc_mem();
}

static void test_tiny(void)
{
	void *ptrs[100];

	for (int i = 0; i < 100; i++) {
		ptrs[i] = malloc(TINY_MAX);
		memset(ptrs[i], 'B', TINY_MAX);
	}

	show_alloc_mem_ex();

	for (int i = 0; i < 100; i++)
		free(ptrs[i]);

	show_alloc_mem();
}

static void test_small(void)
{
	void *ptrs[100];

	for (int i = 0; i < 100; i++) {
		ptrs[i] = malloc(SMALL_MAX);
		memset(ptrs[i], 'B', SMALL_MAX);
	}

	show_alloc_mem_ex();

	for (int i = 0; i < 100; i++)
		free(ptrs[i]);

	show_alloc_mem();
}

static void test_large(void)
{
	void *ptrs[100];

	for (int i = 0; i < 100; i++) {
		ptrs[i] = malloc(SMALL_MAX + 1);
		memset(ptrs[i], 'B', SMALL_MAX + 1);
	}

	show_alloc_mem_ex();

	for (int i = 0; i < 100; i++)
		free(ptrs[i]);

	show_alloc_mem();
}

static void test_realloc(void)
{
	void *a, *b, *c;

	a = malloc(32);
	b = malloc(32);
	c = malloc(32);

	show_alloc_mem_ex();

	free(a);

	show_alloc_mem_ex();

	memset(b, '\0', 32);
	b = realloc(b, 32);

	show_alloc_mem_ex();

	b = realloc(b, 64);

	show_alloc_mem_ex();

	b = realloc(b, 512);

	show_alloc_mem_ex();

	free(b);
	free(c);
}

static void test_fragmentation(void)
{
	void *a, *b, *c, *d;

	a = malloc(32);
	b = malloc(32);
	c = malloc(32);
	d = malloc(32);

	show_alloc_mem_ex();

	free(b);
	free(c);

	show_alloc_mem_ex();

	b = malloc(72);
	c = malloc(32);

	show_alloc_mem_ex();

	free(b);
	free(c);

	show_alloc_mem_ex();

	b = malloc(56);
	c = malloc(32);

	show_alloc_mem_ex();

	free(b);
	free(c);

	show_alloc_mem_ex();

	b = malloc(32);
	c = malloc(32);

	show_alloc_mem_ex();

	free(a);
	free(b);
	free(c);
	free(d);

	show_alloc_mem();
}

int main(void)
{
	test_show();
	test_zero();
	test_basic();
	test_tiny();
	test_small();
	test_large();
	test_realloc();
	test_fragmentation();

	return (0);
}
