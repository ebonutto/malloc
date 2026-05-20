#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Macros */
#define TINY_MAX 128
#define SMALL_MAX 1024

/* Structures */
typedef struct s_chunk {
	int free;
	size_t size;
	struct s_chunk *prev, *next;
} s_chunk;

typedef struct s_zone {
	size_t size;
	s_chunk *chunks;
	struct s_zone *prev, *next;
} s_zone;

/* Prototypes */
void *malloc(size_t size);

#endif
