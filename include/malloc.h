#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Prototypes */
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

void show_alloc_mem(void);
void show_alloc_mem_ex(void);

#endif
