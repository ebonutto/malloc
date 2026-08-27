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

// static void test_realloc(void)
// {
// 	char *ptr;
// 	char *new_ptr;

// 	TEST_HEAD("Test Realloc");

// 	ptr = malloc(16);
// 	if (!ptr)
// 		return (void)TEST_FAIL("Initial malloc()");
// 	memcpy(ptr, "Hello, World!!!", 16);
// 	TEST_PASS("Initial malloc()");

// 	/* Grow */
// 	new_ptr = realloc(ptr, 128);
// 	if (!new_ptr) {
// 		free(ptr);
// 		return (void)TEST_FAIL("Realloc grow");
// 	}
// 	TEST_PASS("Realloc grow");
// 	show_alloc_mem_ex();
// 	ptr = new_ptr;

// 	/* Data preserved */
// 	memcmp(ptr, "Hello, World!!!", 16) == 0 ? TEST_PASS("Realloc data preserved") : TEST_FAIL("Realloc data preserved");

// 	/* Shrink */
// 	new_ptr = realloc(ptr, 8);
// 	if (!new_ptr) {
// 		free(ptr);
// 		return (void)TEST_FAIL("Realloc shrink");
// 	}
// 	TEST_PASS("Realloc shrink");
// 	show_alloc_mem_ex();
// 	ptr = new_ptr;
// 	free(ptr);

// 	/* realloc(NULL) == malloc() */
// 	new_ptr = realloc(NULL, 64);
// 	new_ptr != NULL ? TEST_PASS("realloc(NULL) == malloc()") : TEST_FAIL("realloc(NULL) == malloc()");
// 	free(new_ptr);

// 	show_alloc_mem();
// }

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
	// test_realloc();
	test_fragmentation();

	return (0);
}
