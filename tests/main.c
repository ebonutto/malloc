#include "malloc.h"

#include <stddef.h> // size_t, NULL
#include <string.h> // memset(), strcmp()

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

typedef struct s_test {
	const char *name;
	void (*func)(void);
} t_test;

int main(int argc, char **argv)
{
	t_test tests[] = {
		{"show", test_show},
		{"zero", test_zero},
		{"basic", test_basic},
		{"tiny", test_tiny},
		{"small", test_small},
		{"large", test_large},
		{"realloc", test_realloc},
		{"fragmentation", test_fragmentation},
		{NULL, NULL}
	};
	size_t i;

	if (argc != 2)
		return (1);

	i = 0;
	while (tests[i].name) {
		if (strcmp(argv[1], tests[i].name) == 0) {
			tests[i].func();
			return (0);
		}
		i++;
	}

	return (1);
}
