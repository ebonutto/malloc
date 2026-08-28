#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Prototypes */
void *calloc(size_t count, size_t size) __attribute__((visibility("default")));
void *malloc(size_t size) __attribute__((visibility("default")));
void *realloc(void *ptr, size_t size) __attribute__((visibility("default")));

void free(void *ptr) __attribute__((visibility("default")));

void show_alloc_mem(void) __attribute__((weak, visibility("default")));
void show_alloc_mem_ex(void) __attribute__((weak, visibility("default")));

#endif
