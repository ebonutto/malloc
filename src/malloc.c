#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()

t_zone *g_zone_tiny = NULL;
t_zone *g_zone_small = NULL;
t_zone *g_zone_large = NULL;

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size);

void *malloc(size_t size)
{
	if (size == 0)
		return (NULL);
	size = (size + 15) & ~15;
	if (size <= TINY_MAX)
		return (alloc_in_zone(&g_zone_tiny, size, TINY_ZONE_SIZE));
	return (NULL);
}

static void *alloc_chunk(t_chunk *chunk, size_t size)
{
	t_chunk *leftover;
	size_t old_size;

	old_size = chunk->size;
	chunk->size = size;
	chunk->flags &= ~CHUNK_FREE;

	if (old_size - size >= sizeof(t_chunk) + 16) {
		leftover = (t_chunk *)((char *)chunk + sizeof(t_chunk) + size);
		leftover->size = old_size - size - sizeof(t_chunk);
		leftover->flags = CHUNK_FREE;
		leftover->prev = chunk;
		leftover->next = chunk->next;
		if (chunk->next)
			chunk->next->prev = leftover;
		chunk->next = leftover;
	}
	return ((char *)chunk + sizeof(t_chunk));
}

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size)
{
	t_zone *zone;
	t_chunk *chunk;

	zone = *head;
	while (zone) {
		chunk = zone->chunks;
		while (chunk) {
			if ((chunk->flags & CHUNK_FREE) && chunk->size >= size)
				return (alloc_chunk(chunk, size));
			chunk = chunk->next;
		}
		zone = zone->next;
	}

	zone = create_zone(zone_size);
	if (!zone)
		return (NULL);

	zone->next = *head;
	*head = zone;
	return (alloc_chunk((*head)->chunks, size));
}
