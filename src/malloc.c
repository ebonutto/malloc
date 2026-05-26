#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_malloc_state g_malloc = {NULL, NULL, NULL, NULL, 0};

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size,
                           size_t chunk_type);

void *malloc(size_t size)
{
	if (size == 0)
		return (NULL);
	size = ALIGN16(size);
	if (size <= TINY_MAX)
		return (alloc_in_zone(&g_malloc.tiny, size, TINY_SIZE,
		                      CHUNK_TINY));
	if (size <= SMALL_MAX)
		return (alloc_in_zone(&g_malloc.small, size, SMALL_SIZE,
		                      CHUNK_SMALL));
	return (NULL);
}

static t_chunk *find_free_chunk(t_zone *zone, size_t size)
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

static void *alloc_chunk(t_chunk *chunk, size_t size, size_t chunk_type)
{
	t_chunk *leftover;
	size_t old_size;

	old_size = chunk->size;
	chunk->size = size;
	chunk->flags &= ~CHUNK_FREE;
	if (old_size - size >= CHUNK_HEADER + 16) {
		leftover = (t_chunk *)((char *)chunk + CHUNK_HEADER + size);
		leftover->size = old_size - size - CHUNK_HEADER;
		leftover->flags = CHUNK_FREE | chunk_type;
		leftover->prev = chunk;
		leftover->next = chunk->next;
		if (chunk->next)
			chunk->next->prev = leftover;
		chunk->next = leftover;
	}
	return ((char *)chunk + CHUNK_HEADER);
}

static t_chunk *create_chunk(const t_zone *zone, size_t chunk_type)
{
	t_chunk *chunk;

	chunk = (t_chunk *)((char *)zone + ZONE_HEADER);
	chunk->size = zone->size - ZONE_HEADER - CHUNK_HEADER;
	chunk->flags = CHUNK_FREE | chunk_type;
	chunk->prev = NULL;
	chunk->next = NULL;
	return (chunk);
}

static size_t page_size(void)
{
	static size_t page = 0;

	if (!page)
		page = getpagesize();
	return (page);
}

static t_zone *create_zone(size_t zone_size, size_t chunk_type)
{
	t_zone *zone;

	zone_size = ALIGN_PAGE(zone_size, page_size());
	zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
	            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (perror("mmap()"), NULL);
	zone->size = zone_size;
	zone->prev = NULL;
	zone->next = NULL;
	zone->chunks = create_chunk(zone, chunk_type);
	return (zone);
}

static void zone_prepend(t_zone **head, t_zone *zone)
{
	zone->next = *head;
	if (zone->next)
		zone->next->prev = zone;
	*head = zone;
}

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size,
                           size_t chunk_type)
{
	t_zone *zone;
	t_chunk *chunk;

	chunk = find_free_chunk(*head, size);
	if (chunk)
		return (alloc_chunk(chunk, size, chunk_type));
	zone = create_zone(zone_size, chunk_type);
	if (!zone)
		return (NULL);
	zone_prepend(head, zone);
	return (alloc_chunk((*head)->chunks, size, chunk_type));
}
