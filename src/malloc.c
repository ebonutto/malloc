#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_zone *g_zone_tiny = NULL;
t_zone *g_zone_small = NULL;
t_zone *g_zone_large = NULL;

void *alloc_chunk(t_chunk *chunk, size_t size)
{
	size_t old_size;
	t_chunk *leftover;

	old_size = chunk->size;
	chunk->size = size;
	chunk->free = 0;

	if (old_size - size >= sizeof(t_chunk) + 16) {
		leftover = (t_chunk *)((char *)chunk + sizeof(t_chunk) + size);
		leftover->size = old_size - size - sizeof(t_chunk);
		leftover->free = 1;
		leftover->prev = chunk;
		leftover->next = chunk->next;

		if (chunk->next)
			chunk->next->prev = leftover;
		chunk->next = leftover;
	}

	// Retourner la data, pas le header
	return ((char *)chunk + sizeof(t_chunk));
}

void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size)
{
	t_zone  *zone;
	t_chunk *chunk;

	zone = *head;
	while (zone) {
		chunk = zone->chunks;
		while (chunk) {
			if (chunk->free && chunk->size >= size)
				return (alloc_chunk(chunk, size));
			chunk = chunk->next;
		}
		zone = zone->next;
	}

	// Rien trouvé → nouvelle zone
	zone = create_zone(zone_size);  // ← zone_size pas size !
	if (!zone)
		return NULL;
	zone->next = *head;
	*head      = zone;

	return alloc_chunk((*head)->chunks, size);
}

void *malloc(size_t size)
{
	return (alloc_in_zone(&g_zone_tiny, size, TINY_ZONE_SIZE));
	return (NULL);
}
