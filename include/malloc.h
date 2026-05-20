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

/* Global variables */
static s_zone *g_zone_tiny = NULL;
static s_zone *g_zone_small = NULL;
static s_zone *g_zone_large = NULL;

/* Prototypes */
void *malloc(size_t size);

#endif
