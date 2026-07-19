#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t
#include <stdio.h> // printf()

static size_t show_chunks(t_chunk *chunk)
{
	size_t total;
	void *start;
	void *end;

	total = 0;
	while (chunk) {
		if (!(chunk->flags & CHUNK_FREE)) {
			start = (void *)(chunk + 1);
			end = (void *)((char *)start + chunk->size);
			printf("%p - %p : %zu bytes\n", start, end,
			       chunk->size);
			total += chunk->size;
		}
		chunk = chunk->next;
	}
	return (total);
}

static size_t show_zones(t_zone *g_zone, const char *zone_name)
{
	t_zone *zone;
	size_t total;

	printf("%s : %p\n", zone_name, (void *)g_zone);
	total = 0;
	zone = g_zone;
	while (zone) {
		total += show_chunks(zone->chunks);
		zone = zone->next;
	}
	return (total);
}

static void show_alloc_mem_impl(void)
{
	size_t total;

	total = 0;
	total += show_zones(g_malloc.tiny, "TINY");
	total += show_zones(g_malloc.small, "SMALL");
	total += show_zones(g_malloc.large, "LARGE");
	printf("Total : %zu byte(s)\n", total);
}

void show_alloc_mem(void)
{
	pthread_mutex_lock(&g_malloc.lock);
	show_alloc_mem_impl();
	pthread_mutex_unlock(&g_malloc.lock);
}
