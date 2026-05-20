#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Macros */
#define TINY_MAX 128
#define SMALL_MAX 1024

/* Structures */
typedef struct s_chunk {
	size_t size;
	struct s_chunk *next;
	struct s_chunk *prev;
	unsigned char free;
} t_chunk;

typedef struct s_zone {
	size_t total_size;
	struct s_chunk *chunks;
	struct s_zone *next;
} t_zone;

/* Global variables */
static t_zone *g_zone_tiny = NULL;
static t_zone *g_zone_small = NULL;
static t_zone *g_zone_large = NULL;

/* Prototypes */
void *malloc(size_t size);

#endif
