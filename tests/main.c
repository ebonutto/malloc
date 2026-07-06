#include <stddef.h> // NULL
#include <stdio.h> // printf()
#include <string.h> // memset()

#include "malloc.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

#define TINY_MAX 128
#define SMALL_MAX 1024

#define TEST_TITLE(name) printf(YELLOW  "\n--- %s ---\n" RESET, name)
#define TEST_HEAD(name) printf(BLUE  "\n--- %s ---\n" RESET, name)
#define TEST_PASS(name) printf(GREEN "[PASS]" RESET " %s\n", name)
#define TEST_FAIL(name) printf(RED   "[FAIL]" RESET " %s\n", name)

//static void test_zero(void)
//{
//	void *ptr;

//	TEST_HEAD("Test Zero");

//	/* malloc */
//	ptr = malloc(0);
//	ptr == NULL ? TEST_PASS("malloc(0) = NULL") : TEST_FAIL("malloc(0) != NULL");

//	/* calloc */
//	ptr = calloc(42, 0);
//	ptr == NULL ? TEST_PASS("calloc(42, 0) = NULL") : TEST_FAIL("calloc(42, 0) != NULL");
//	ptr = calloc(0, 42);
//	ptr == NULL ? TEST_PASS("calloc(0, 42) = NULL") : TEST_FAIL("calloc(0, 42) != NULL");
//	ptr = calloc(0, 0);
//	ptr == NULL ? TEST_PASS("calloc(0, 0) = NULL") : TEST_FAIL("calloc(0, 0) != NULL");

//	/* realloc */
//	ptr = realloc(ptr, 0);
//	ptr == NULL ? TEST_PASS("realloc(ptr, 0) = NULL") : TEST_FAIL("realloc(ptr, 0) != NULL");
//	//ptr = realloc(NULL, 42);
//	//ptr == NULL ? TEST_PASS("realloc(NULL, 42) = NULL") : TEST_FAIL("realloc(NULL, 42) != NULL");
//	ptr = realloc(NULL, 0);
//	ptr == NULL ? TEST_PASS("realloc(NULL, 0) = NULL") : TEST_FAIL("realloc(NULL, 0) != NULL");

//	/* realloc */
//	free(NULL);
//	TEST_PASS("free(NULL) no crash");

//	show_alloc_mem(); // No allocated memory expected
//}

//static void test_tiny(void)
//{
//	void *ptrs[100];
//	size_t i;

//	TEST_HEAD("Test Tiny");

//	/* Allocate memory */
//	i = 0;
//	while (i < 100) {
//		ptrs[i] = malloc(TINY_MAX);
//		if (!ptrs[i]) {
//			TEST_FAIL("malloc(TINY_MAX) failed");
//			while (i--)
//				free(ptrs[i]);
//			return ;
//		}
//		memset(ptrs[i], 42, TINY_MAX);
//		i++;
//	}
//	TEST_PASS("malloc(TINY_MAX) x 100");
//	show_alloc_mem_ex();

//	/* Free memory */
//	while (i--)
//		free(ptrs[i]);
//	TEST_PASS("free(tiny) x 100");
//	show_alloc_mem();
//}

//static void test_small(void)
//{
//	void *ptrs[100];
//	size_t i;

//	TEST_HEAD("Test Small");

//	/* Allocate memory */
//	i = 0;
//	while (i < 100) {
//		ptrs[i] = malloc(SMALL_MAX);
//		if (!ptrs[i]) {
//			TEST_FAIL("malloc(SMALL_MAX) failed");
//			while (i--)
//				free(ptrs[i]);
//			return ;
//		}
//		memset(ptrs[i], 42, SMALL_MAX);
//		i++;
//	}
//	TEST_PASS("malloc(SMALL_MAX) x 100");
//	show_alloc_mem_ex();

//	/* Free memory */
//	while (i--)
//		free(ptrs[i]);
//	TEST_PASS("free(tiny) x 100");
//	show_alloc_mem();
//}

//static void test_large(void)
//{
//	void *ptrs[100];
//	size_t i;

//	TEST_HEAD("Test Large");

//	/* Allocate memory */
//	i = 0;
//	while (i < 100) {
//		ptrs[i] = malloc(1025);
//		if (!ptrs[i]) {
//			TEST_FAIL("malloc(1025) failed");
//			while (i--)
//				free(ptrs[i]);
//			return ;
//		}
//		memset(ptrs[i], 42, 1025);
//		i++;
//	}
//	TEST_PASS("malloc(1025) x 100");
//	show_alloc_mem_ex();

//	/* Free memory */
//	while (i--)
//		free(ptrs[i]);
//	TEST_PASS("free(tiny) x 100");
//	show_alloc_mem();
//}

static void test_fragmentation(void)
{
	void *a;
	void *b;
	void *c;
	void *d;

	TEST_HEAD("Test Fragmentation");

	a = malloc(64);
	b = malloc(64);
	c = malloc(64);
	d = malloc(64);
	if (!a || !b || !c || !d) {
		TEST_FAIL("malloc() failed");
		goto error;
	}
	show_alloc_mem_ex();

	/* Free middle */
	free(b);
	free(c);
	show_alloc_mem_ex();

	/* Re-using same space */
	b = malloc(128);
	c = malloc(64);
	if (!b || !c) {
		TEST_FAIL("malloc() failed");
		goto error;
	}
	show_alloc_mem_ex();

	free(b);
	free(c);
	show_alloc_mem_ex();

	free(d);
	show_alloc_mem_ex();

	free(a);

	show_alloc_mem();
	return ;
error:
	free(a);
	free(b);
	free(c);
	free(d);
	show_alloc_mem();
}

/////* ─────────────────────────────────────────────
////** 6. REALLOC
////** ───────────────────────────────────────────── */
////static void test_realloc(void)
////{
////	char *ptr;
////	char *new_ptr;

////	TEST_HEAD("REALLOC");

////	ptr = malloc(16);
////	if (!ptr)
////	{
////		TEST_FAIL("realloc initial malloc");
////		return ;
////	}
////	memcpy(ptr, "Hello, World!!!", 16);

////	/* Agrandir */
////	new_ptr = realloc(ptr, 128);
////	if (!new_ptr)
////	{
////		TEST_FAIL("realloc grow");
////		free(ptr);
////		return ;
////	}
////	memcmp(new_ptr, "Hello, World!!!", 16) == 0
////		? TEST_PASS("realloc grow data preserved")
////		: TEST_FAIL("realloc grow data preserved");
////	ptr = new_ptr;

////	/* Réduire */
////	new_ptr = realloc(ptr, 8);
////	new_ptr != NULL
////		? TEST_PASS("realloc shrink")
////		: TEST_FAIL("realloc shrink");
////	ptr = new_ptr;

////	/* realloc(NULL) == malloc */
////	new_ptr = realloc(NULL, 64);
////	new_ptr != NULL
////		? TEST_PASS("realloc(NULL) == malloc")
////		: TEST_FAIL("realloc(NULL) == malloc");
////	free(new_ptr);

////	/* realloc(ptr, 0) == free */
////	new_ptr = realloc(ptr, 0);
////	new_ptr == NULL
////		? TEST_PASS("realloc(ptr, 0) == free")
////		: TEST_FAIL("realloc(ptr, 0) == free");

////	show_alloc_mem();
////}

///* Useful to test history and hexdump */
//static void test_show(void)
//{
//	void *a;
//	void *b;

//	TEST_HEAD("Start Test Show");

//	/* malloc + memset */
//	a = malloc(32);
//	b = malloc(512);
//	memset(a, 'A', 32);
//	memset(b, 'B', 512);

//	show_alloc_mem();
//	TEST_PASS("show_alloc_mem()");
//	show_alloc_mem_ex();
//	TEST_PASS("show_alloc_mem_ex()");

//	/* free */
//	free(a);
//	free(b);

//	show_alloc_mem();
//	TEST_PASS("show_alloc_mem()");
//	show_alloc_mem_ex();
//	TEST_PASS("show_alloc_mem_ex()");

//	TEST_HEAD("End Test Show");
//}

int main(void)
{
	TEST_TITLE("Start Test malloc");

	//test_zero();
	//test_basic();
	//test_tiny();
	//test_small();
	//test_large();
	//test_stress();
	test_fragmentation();
	//test_show();

	TEST_TITLE("End Test malloc");
	return (0);
}
