#ifndef MALLOC_INT_H
#define MALLOC_INT_H

/* Includes */
#include "malloc.h"

#include <pthread.h> // pthread_mutex_t
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

typedef struct s_log {
	const char op;
	void *ptr;
	size_t size;
	struct s_log *next;
} t_log;

typedef struct s_malloc_state {
	t_zone *tiny;
	t_zone *small;
	t_zone *large;
	t_log *log;
	pthread_mutex_t lock;
	int flags;
} t_malloc_state;

/* Macros */
#define ZONE_HEADER sizeof(t_zone)
#define CHUNK_HEADER sizeof(t_chunk)

#define ALIGN16(size) (((size) + 15) & ~15)
#define ALIGN_PAGE(size, page) (((size) + (page) - 1) & ~((page) - 1))

#define TINY_MAX 128
#define SMALL_MAX 1024

#define TINY_SIZE (ZONE_HEADER + (CHUNK_HEADER + TINY_MAX) * 100)
#define SMALL_SIZE (ZONE_HEADER + (CHUNK_HEADER + SMALL_MAX) * 100)

#define CHUNK_FREE (1 << 0)
#define CHUNK_TINY (1 << 1)
#define CHUNK_SMALL (1 << 2)
#define CHUNK_LARGE (1 << 3)

/* Global variables */
extern t_malloc_state g_malloc;

/* Prototypes */
/* chunk.c */
t_chunk *create_chunk(const t_zone *zone, size_t chunk_type);
t_chunk *find_free_chunk(t_zone *zone, size_t size);
void *alloc_chunk(t_chunk *chunk, size_t size, size_t chunk_type);

/* zone.c */
t_zone *create_zone(size_t zone_size, size_t chunk_type);
void zone_prepend(t_zone **head, t_zone *zone);

/* malloc.c */
void *malloc_impl(size_t size);

/* free.c */
void free_impl(void *ptr);

#endif
