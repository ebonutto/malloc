#ifndef MALLOC_H
#define MALLOC_H

/* Includes */
#include <stddef.h> // size_t

/* Structures */
typedef struct s_chunk {
	size_t size;
	size_t flags;
	struct s_chunk *prev;
	struct s_chunk *next;
} t_chunk;

typedef struct s_zone {
	size_t size;
	t_chunk *chunks;
	struct s_zone *prev;
	struct s_zone *next;
} t_zone;

/* Macros */
#define ZONE_HEADER sizeof(t_zone)
#define CHUNK_HEADER sizeof(t_chunk)

#define ALIGN16(size) (((size) + 15) & ~15)
#define ALIGN_PAGE(size, page) (((size) + (page) - 1) & ~((page) - 1))

#define TINY_MAX 128
#define SMALL_MAX 1024

#define TINY_SIZE ((TINY_MAX + ZONE_HEADER) * 100)
#define SMALL_SIZE ((SMALL_MAX + CHUNK_HEADER) * 100)

#define CHUNK_FREE (1 << 0)
#define CHUNK_TINY (1 << 1)
#define CHUNK_SMALL (1 << 2)
#define CHUNK_LARGE (1 << 3)

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
