#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Macros */
#define TINY_MAX 128
#define SMALL_MAX 1024

#define TINY_ZONE_SIZE TINY_MAX * 100

#define CHUNK_FREE (1 << 0) // 0b001
#define CHUNK_TINY (1 << 1) // 0b010
#define CHUNK_SMALL (1 << 2) // 0b100
#define CHUNK_LARGE (1 << 3) // 0b1000


/* Structures */
typedef struct s_chunk {
	size_t size;
	struct s_chunk *prev;
	struct s_chunk *next;
	size_t flags;
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

/* print.c */
void show_alloc_mem(void);

/* zone.c */
t_zone *create_zone(size_t size);

#endif
