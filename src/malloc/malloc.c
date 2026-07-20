#include "malloc_int.h"

#include <pthread.h> // PTHREAD_MUTEX_INITIALIZER, pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t, NULL

t_malloc_state g_malloc = {
	PTHREAD_MUTEX_INITIALIZER,
	NULL,
	NULL,
	NULL,
	{0},
	0
};

static void *alloc_in_new_zone(t_zone **head, size_t zone_size, size_t size,
                               size_t chunk_type)
{
	t_zone *zone;

	zone = create_zone(zone_size, chunk_type);
	if (!zone)
		return (NULL);

	zone_prepend(head, zone);

	return (alloc_chunk(zone->chunks, size, chunk_type));
}

static void *alloc_in_zone(t_zone **head, size_t zone_size, size_t size,
                           size_t chunk_type)
{
	t_chunk *chunk;

	chunk = find_free_chunk(*head, size);
	if (chunk)
		return (alloc_chunk(chunk, size, chunk_type));

	return (alloc_in_new_zone(head, zone_size, size, chunk_type));
}

static void *alloc_large(size_t size)
{
	t_zone *zone;

	zone = create_zone(ZONE_HEADER + CHUNK_HEADER + size, CHUNK_LARGE);
	if (!zone)
		return (NULL);

	zone->chunks->flags &= ~CHUNK_FREE;

	zone_prepend(&g_malloc.large, zone);

	return ((char *)zone + ZONE_HEADER + CHUNK_HEADER);
}

void *malloc_impl(size_t size)
{
	if (size == 0)
		return (NULL);

	size = ALIGN16(size);

	if (size <= TINY_MAX)
		return (alloc_in_zone(&g_malloc.tiny, TINY_SIZE, size,
		                      CHUNK_TINY));

	if (size <= SMALL_MAX)
		return (alloc_in_zone(&g_malloc.small, SMALL_SIZE, size,
		                      CHUNK_SMALL));

	return (alloc_large(size));
}

void *malloc(size_t size)
{
	void *ptr;

	pthread_mutex_lock(&g_malloc.lock);

	ptr = malloc_impl(size);

	if (g_malloc.flags & MALLOC_HISTORY)
		history_push(LOG_MALLOC, ptr, NULL, size);

	pthread_mutex_unlock(&g_malloc.lock);

	return (ptr);
}
