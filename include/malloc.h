#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Macros */
#define TINY_MAX 128
#define SMALL_MAX 1024

#define TINY_SIZE ((TINY_MAX + sizeof(t_zone)) * 100)
#define SMALL_SIZE ((SMALL_MAX + sizeof(t_zone)) * 100)

#define CHUNK_FREE (1 << 0)
#define CHUNK_TINY (1 << 1)
#define CHUNK_SMALL (1 << 2)
#define CHUNK_LARGE (1 << 3)

/* Structures */
typedef struct s_chunk {
	size_t size;
	struct s_chunk *prev;
	struct s_chunk *next;
	size_t flags;
} t_chunk;

typedef struct s_zone {
	size_t size;
	t_chunk *chunks;
	struct s_zone *prev, *next;
} t_zone;

/* Global variables */
extern t_zone *g_tiny_zones;
extern t_zone *g_small_zones;
extern t_zone *g_large_zones;

/* Prototypes */
/* free.c */
void free(void *ptr);

/* malloc.c */
void *malloc(size_t size);

/* print.c */
void show_alloc_mem(void);

#endif
