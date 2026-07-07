#include "malloc.h"

#include <stddef.h> // NULL
#include <stdio.h> // printf()
#include <string.h> // memcmp(), memcpy(), memset()

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

#define TINY_MAX 128 //TODO Update if 32 or 64 bits
#define SMALL_MAX 1024 //TODO Update if 32 or 64 bits

#define TEST_TITLE(name) printf(YELLOW  "\n--- %s ---\n" RESET, name)
#define TEST_HEAD(name) printf(BLUE  "\n--- %s ---\n" RESET, name)
#define TEST_PASS(name) printf(GREEN "[PASS]" RESET " %s\n", name)
#define TEST_FAIL(name) printf(RED   "[FAIL]" RESET " %s\n", name)

/* Useful to test history and hexdump */
static void test_show(void)
{
	void *a;
	void *b;

	TEST_HEAD("Test Show");

	/* malloc + memset */
	a = malloc(32);
	b = malloc(512);
	memset(a, 'A', 32);
	memset(b, 'B', 512);

	show_alloc_mem();
	TEST_PASS("show_alloc_mem()");
	show_alloc_mem_ex();
	TEST_PASS("show_alloc_mem_ex()");

	/* free */
	free(a);
	free(b);

	show_alloc_mem();
	TEST_PASS("show_alloc_mem()");
	show_alloc_mem_ex();
	TEST_PASS("show_alloc_mem_ex()");
}

static void test_basic(void)
{
	void *a;
	void *b;
	void *c;

	TEST_HEAD("Test Basic");

	/* malloc() x 3 */
	a = malloc(1);
	b = malloc(42);
	c = malloc(512);
	if (!a || !b || !c) {
		TEST_FAIL("malloc() x 3");
		goto cleanup;
	}
	TEST_PASS("malloc() x 3");
	show_alloc_mem_ex();

cleanup:
	free(a);
	free(b);
	free(c);
	show_alloc_mem();
}

static void test_zero(void)
{
	void *ptr;

	TEST_HEAD("Test Zero");

	/* malloc */
	ptr = malloc(0);
	ptr == NULL ? TEST_PASS("malloc(0) = NULL") : TEST_FAIL("malloc(0) != NULL");

	/* calloc */
	ptr = calloc(42, 0);
	ptr == NULL ? TEST_PASS("calloc(42, 0) = NULL") : TEST_FAIL("calloc(42, 0) != NULL");
	ptr = calloc(0, 42);
	ptr == NULL ? TEST_PASS("calloc(0, 42) = NULL") : TEST_FAIL("calloc(0, 42) != NULL");
	ptr = calloc(0, 0);
	ptr == NULL ? TEST_PASS("calloc(0, 0) = NULL") : TEST_FAIL("calloc(0, 0) != NULL");

	/* realloc */
	ptr = realloc(NULL, 0);
	ptr == NULL ? TEST_PASS("realloc(NULL, 0) = NULL") : TEST_FAIL("realloc(NULL, 0) != NULL");

	/* realloc */
	free(NULL);
	TEST_PASS("free(NULL) no crash");

	show_alloc_mem(); // No allocated memory expected
}

static void test_tiny(void)
{
	void *ptrs[100];
	size_t i;

	TEST_HEAD("Test Tiny");

	/* Allocate memory */
	i = 0;
	while (i < 100) {
		ptrs[i] = malloc(TINY_MAX);
		if (!ptrs[i]) {
			TEST_FAIL("malloc(TINY_MAX) failed");
			while (i--)
				free(ptrs[i]);
			return ;
		}
		memset(ptrs[i], 42, TINY_MAX);
		i++;
	}
	TEST_PASS("malloc(TINY_MAX) x 100");
	show_alloc_mem_ex();

	/* Free memory */
	while (i--)
		free(ptrs[i]);
	TEST_PASS("free(tiny) x 100");
	show_alloc_mem();
}

static void test_small(void)
{
	void *ptrs[100];
	size_t i;

	TEST_HEAD("Test Small");

	/* Allocate memory */
	i = 0;
	while (i < 100) {
		ptrs[i] = malloc(SMALL_MAX);
		if (!ptrs[i]) {
			TEST_FAIL("malloc(SMALL_MAX) failed");
			while (i--)
				free(ptrs[i]);
			return ;
		}
		memset(ptrs[i], 42, SMALL_MAX);
		i++;
	}
	TEST_PASS("malloc(SMALL_MAX) x 100");
	show_alloc_mem_ex();

	/* Free memory */
	while (i--)
		free(ptrs[i]);
	TEST_PASS("free(tiny) x 100");
	show_alloc_mem();
}

static void test_large(void)
{
	void *ptrs[100];
	size_t i;

	TEST_HEAD("Test Large");

	/* Allocate memory */
	i = 0;
	while (i < 100) {
		ptrs[i] = malloc(1025);
		if (!ptrs[i]) {
			TEST_FAIL("malloc(1025) failed");
			while (i--)
				free(ptrs[i]);
			return ;
		}
		memset(ptrs[i], 42, 1025);
		i++;
	}
	TEST_PASS("malloc(1025) x 100");
	show_alloc_mem_ex();

	/* Free memory */
	while (i--)
		free(ptrs[i]);
	TEST_PASS("free(tiny) x 100");
	show_alloc_mem();
}

static void test_realloc(void)
{
	char *ptr;
	char *new_ptr;

	TEST_HEAD("Test Realloc");

	ptr = malloc(16);
	if (!ptr)
		return (void)TEST_FAIL("Initial malloc()");
	memcpy(ptr, "Hello, World!!!", 16);
	TEST_PASS("Initial malloc()");

	/* Grow */
	new_ptr = realloc(ptr, 128);
	if (!new_ptr) {
		free(ptr);
		return (void)TEST_FAIL("Realloc grow");
	}
	TEST_PASS("Realloc grow");
	show_alloc_mem_ex();
	ptr = new_ptr;

	/* Data preserved */
	memcmp(ptr, "Hello, World!!!", 16) == 0 ? TEST_PASS("Realloc data preserved") : TEST_FAIL("Realloc data preserved");

	/* Shrink */
	new_ptr = realloc(ptr, 8);
	if (!new_ptr) {
		free(ptr);
		return (void)TEST_FAIL("Realloc shrink");
	}
	TEST_PASS("Realloc shrink");
	show_alloc_mem_ex();
	ptr = new_ptr;
	free(ptr);

	/* realloc(NULL) == malloc() */
	new_ptr = realloc(NULL, 64);
	new_ptr != NULL ? TEST_PASS("realloc(NULL) == malloc()") : TEST_FAIL("realloc(NULL) == malloc()");
	free(new_ptr);

	show_alloc_mem();
}

static void test_fragmentation(void)
{
	void *a;
	void *b;
	void *c;
	void *d;

	TEST_HEAD("Test Fragmentation");

	/* Initial setup */
	a = malloc(32);
	b = malloc(32);
	c = malloc(32);
	d = malloc(32);
	if (!a || !b || !c || !d) {
		TEST_FAIL("malloc() failed");
		goto cleanup;
	}
	TEST_PASS("Initial setup");
	show_alloc_mem_ex();

	/* Free middle */
	free(b);
	free(c);
	TEST_PASS("Free middle");
	show_alloc_mem_ex();

	/* Fat chunk in the middle */
	b = malloc(72);
	c = malloc(32);
	if (!b || !c) {
		TEST_FAIL("malloc() failed");
		goto cleanup;
	}
	TEST_PASS("Fat chunk in the middle");
	show_alloc_mem_ex();

	/* Free middle */
	free(b);
	free(c);
	TEST_PASS("Free middle");
	show_alloc_mem_ex();

	/* Chunk with 0 data in the middle */
	b = malloc(56);
	c = malloc(32);
	if (!b || !c) {
		TEST_FAIL("malloc() failed");
		goto cleanup;
	}
	TEST_PASS("Chunk with 0 data in the middle");
	show_alloc_mem_ex();

	/* Free middle */
	free(b);
	free(c);
	TEST_PASS("Free middle");
	show_alloc_mem_ex();

	/* Go back to initial setup */
	b = malloc(32);
	c = malloc(32);
	if (!b || !c) {
		TEST_FAIL("malloc() failed");
		goto cleanup;
	}
	TEST_PASS("Go back to initial setup");
	show_alloc_mem_ex();

	/* Free all */
cleanup:
	free(a);
	free(b);
	free(c);
	free(d);
	TEST_PASS("Free all");
	show_alloc_mem();
}

int main(void)
{
	TEST_TITLE("Start Test malloc");

	test_show();
	test_zero();
	test_basic();
	test_tiny();
	test_small();
	test_large();
	test_realloc();
	test_fragmentation();

	TEST_TITLE("End Test malloc");
	return (0);
}
