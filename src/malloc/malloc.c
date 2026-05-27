#include "malloc.h"
#include "malloc_int.h"

#include <stddef.h> // size_t, NULL

t_malloc_state g_malloc = {NULL, NULL, NULL, NULL, 0};

static void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size,
                           size_t chunk_type);
void *alloc_large(t_zone **head, size_t size);

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
	return (alloc_large(&g_malloc.large, size));
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

void *alloc_large(t_zone **head, size_t size)
{
	t_zone *zone;

	zone = create_zone(size, CHUNK_LARGE);
	if (!zone)
		return (NULL);
	zone_prepend(head, zone);
	return (alloc_chunk((*head)->chunks, size, CHUNK_LARGE));
}
