#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Macros */
#define TINY_MAX 128
#define SMALL_MAX 1024

#define TINY_ZONE_SIZE TINY_MAX * 100

/* Structures */
typedef struct s_chunk {
	size_t size;
	struct s_chunk *prev;
	struct s_chunk *next;
	unsigned char free;
} t_chunk;

typedef struct s_zone {
	size_t total_size;
	t_chunk *chunks;
	struct s_zone *next;
} t_zone;

/* Global variables */
extern t_zone *g_zone_tiny;
extern t_zone *g_zone_small;
extern t_zone *g_zone_large;

/* Prototypes */
/* malloc.c */
void *malloc(size_t size);

/* free.c */
void free(void *ptr);

/* zone.c */
t_zone *create_zone(size_t size);

/* print.c */
void show_alloc_mem_ex(void);

#endif
