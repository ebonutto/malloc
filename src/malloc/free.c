#include "malloc.h"
#include "malloc_int.h"

#include <sys/mman.h> // munmap()

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // NULL

static void coalesce(t_chunk *curr)
{
	t_chunk *prev;
	t_chunk *next;

	prev = curr->prev;
	next = curr->next;

	if (prev && (prev->flags & CHUNK_FREE)) {
		prev->size += CHUNK_HEADER + curr->size;
		prev->next = next;

		if (next)
			next->prev = prev;

		curr = prev;
	}

	if (next && (next->flags & CHUNK_FREE)) {
		curr->size += CHUNK_HEADER + next->size;
		curr->next = next->next;

		if (next->next)
			next->next->prev = curr;
	}
}

void free_impl(void *ptr)
{
	t_chunk *chunk;
	t_zone *zone;

	if (!ptr)
		return ;

	chunk = (t_chunk *)((char *)ptr - CHUNK_HEADER);

	if (chunk->flags & CHUNK_LARGE) {
		zone = (t_zone *)((char *)chunk - ZONE_HEADER);

		if (zone->prev)
			zone->prev->next = zone->next;
		else
			g_malloc.large = zone->next;

		if (zone->next)
			zone->next->prev = zone->prev;

		munmap(zone, zone->size);
		return ;
	}

	if (chunk->flags & CHUNK_FREE)
		return ;

	chunk->flags |= CHUNK_FREE;
	coalesce(chunk);
}

void free(void *ptr)
{
	pthread_mutex_lock(&g_malloc.lock);

	free_impl(ptr);

	if (g_malloc.flags & MALLOC_HISTORY)
		history_push(LOG_FREE, ptr, NULL, 0);

	pthread_mutex_unlock(&g_malloc.lock);
}

static void free_zone_list(t_zone *zone)
{
	t_zone *curr;
	t_zone *next;

	curr = zone;
	while (curr) {
		next = curr->next;
		munmap(curr, curr->size);
		curr = next;
	}
}

__attribute__((destructor))
static void free_all_zones(void)
{
	free_zone_list(g_malloc.tiny);
	free_zone_list(g_malloc.small);
	free_zone_list(g_malloc.large);
}
