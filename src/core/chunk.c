#include "malloc_int.h"

#include <stddef.h> // size_t, NULL

t_chunk *create_chunk(const t_zone *zone, size_t chunk_type)
{
	t_chunk *chunk;

	chunk = (t_chunk *)((char *)zone + ZONE_HEADER);

	chunk->size = zone->size - ZONE_HEADER - CHUNK_HEADER;
	chunk->flags = chunk_type | CHUNK_FREE;
	chunk->prev = NULL;
	chunk->next = NULL;

	return (chunk);
}

t_chunk *find_free_chunk(t_zone *zone, size_t size)
{
	t_chunk *chunk;

	while (zone) {
		chunk = zone->chunks;
		while (chunk) {
			if ((chunk->flags & CHUNK_FREE) && chunk->size >= size)
				return (chunk);

			chunk = chunk->next;
		}

		zone = zone->next;
	}

	return (NULL);
}

void *alloc_chunk(t_chunk *chunk, size_t size, size_t chunk_type)
{
	t_chunk *leftover;
	size_t old_size;

	old_size = chunk->size;
	chunk->flags &= ~CHUNK_FREE;
	if (old_size - size >= CHUNK_HEADER) {
		chunk->size = size;
		leftover = (t_chunk *)(CHUNK_HEADER + (char *)chunk + size);
		leftover->size = old_size - size - CHUNK_HEADER;
		leftover->flags = CHUNK_FREE | chunk_type;
		leftover->prev = chunk;
		leftover->next = chunk->next;
		if (chunk->next)
			chunk->next->prev = leftover;
		chunk->next = leftover;
	}

	return (CHUNK_HEADER + (char *)chunk);
}
