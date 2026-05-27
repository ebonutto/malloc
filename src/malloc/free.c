#include "malloc_int.h"

#include <sys/mman.h> // munmap()

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()

static void coalesce(t_chunk *curr)
{
	t_chunk *prev;
	t_chunk *next;

	prev = curr->prev;
	next = curr->next;
	if (prev && (prev->flags & CHUNK_FREE)) {
		prev->size += curr->size + CHUNK_HEADER;
		prev->next = next;
		if (next)
			next->prev = prev;
		curr = prev;
	}
	if (next && (next->flags & CHUNK_FREE)) {
		curr->size += next->size + CHUNK_HEADER;
		curr->next = next->next;
		if (next->next)
			next->next->prev = curr;
	}
}

static t_zone **get_zone_head(t_chunk *chunk)
{
	if (chunk->flags & CHUNK_TINY)
		return (&g_malloc.tiny);
	if (chunk->flags & CHUNK_SMALL)
		return (&g_malloc.small);
	return (&g_malloc.large);
}

static void zone_unlink(t_zone **head, t_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	else
		*head = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;
}

static void free_impl(void *ptr)
{
	t_chunk *curr;
	t_zone *zone;

	if (!ptr)
		return ;
	curr = (t_chunk *)((char *)ptr - CHUNK_HEADER);
	if (curr->flags & CHUNK_FREE)
		return ;
	curr->flags |= CHUNK_FREE;
	coalesce(curr);
	if (!curr->prev && !curr->next) {
		zone = (t_zone *)((char *)curr - ZONE_HEADER);
		zone_unlink(get_zone_head(curr), zone);
		munmap(zone, zone->size);
	}
}

void free(void *ptr)
{
	pthread_mutex_lock(&g_malloc.lock);
	free_impl(ptr);
	pthread_mutex_unlock(&g_malloc.lock);
}
