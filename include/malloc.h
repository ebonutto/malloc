#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Prototypes */
/* malloc.c */
void *malloc(size_t size);

/* free.c */
void free(void *ptr);

/* realloc.c */
void *realloc(void *ptr, size_t size);

/* show.c */
void show_alloc_mem(void);

/* show_ex.c */
void show_alloc_mem_ex(void);

#endif
