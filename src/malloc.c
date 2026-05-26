#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_zone *g_tiny_zones = NULL;
t_zone *g_small_zones = NULL;
t_zone *g_large_zones = NULL;

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size,
                           size_t chunk_type);
static t_zone *create_zone(size_t zone_size, size_t chunk_type);

void *malloc(size_t size)
{
	if (size == 0)
		return (NULL);

	size = (size + 15) & ~15; // 16-byte alignment

	if (size <= TINY_MAX)
		return (alloc_in_zone(&g_tiny_zones, size, TINY_SIZE,
                                      CHUNK_TINY));
	if (size <= SMALL_MAX)
		return (alloc_in_zone(&g_small_zones, size, SMALL_SIZE,
                                      CHUNK_SMALL));
	return (NULL);
}

static void *alloc_chunk(t_chunk *chunk, size_t size, size_t chunk_type)
{
	t_chunk *leftover;
	size_t old_size;

	old_size = chunk->size;
	chunk->size = size;
	chunk->flags &= ~CHUNK_FREE;

	if (old_size - size >= sizeof(t_chunk) + 16) {
		leftover = (t_chunk *)((char *)chunk + sizeof(t_chunk) + size);

		leftover->size = old_size - size - sizeof(t_chunk);
		leftover->flags = CHUNK_FREE | chunk_type;
		leftover->prev = chunk;

		leftover->next = chunk->next;
		if (chunk->next)
			chunk->next->prev = leftover;

		chunk->next = leftover;
	}

	return ((char *)chunk + sizeof(t_chunk));
}

static t_zone *create_zone(size_t zone_size, size_t chunk_type)
{
	t_zone *zone;
	t_chunk *chunk;
	size_t page;

	page = getpagesize();
	zone_size = (zone_size + page - 1) & ~(page - 1);

	zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
	            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (perror("mmap()"), NULL);

	zone->total_size = zone_size;
	zone->next = NULL;

	chunk = (t_chunk *)((char *)zone + sizeof(t_zone));
	chunk->size = zone->total_size - sizeof(t_zone) - sizeof(t_chunk);
	chunk->flags = CHUNK_FREE | chunk_type;
	chunk->next = NULL;
	chunk->prev = NULL;

	zone->chunks = chunk;
	return (zone);
}

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size,
                           size_t chunk_type)
{
	t_zone *zone;
	t_chunk *chunk;

	zone = *head;
	while (zone) {
		chunk = zone->chunks;
		while (chunk) {
			if ((chunk->flags & CHUNK_FREE) && chunk->size >= size)
				return (alloc_chunk(chunk, size, chunk_type));
			chunk = chunk->next;
		}
		zone = zone->next;
	}

	zone = create_zone(zone_size, chunk_type);
	if (!zone)
		return (NULL);

	zone->next = *head;
	*head = zone;
	return (alloc_chunk((*head)->chunks, size, chunk_type));
}
