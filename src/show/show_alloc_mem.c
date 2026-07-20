#include "malloc.h"
#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t

static size_t show_chunks(t_chunk *chunk)
{
	size_t total;
	void *start;
	void *end;

	total = 0;
	while (chunk) {
		if (!(chunk->flags & CHUNK_FREE)) {
			start = (char *)chunk + sizeof(t_chunk);
			end = (char *)start + chunk->size;

			putptr(start);
			putstr(" - ");
			putptr(end);
			putstr(" : ");
			putnbr(chunk->size);
			putstr(" byte(s)\n");

			total += chunk->size;
		}
		chunk = chunk->next;
	}

	return (total);
}

static size_t show_zones(t_zone *zone, const char *name)
{
	size_t total;

	putstr(name);
	putstr(" : ");
	putptr(zone);
	putchar('\n');

	total = 0;
	while (zone) {
		total += show_chunks(zone->chunks);
		zone = zone->next;
	}

	return (total);
}

void show_alloc_mem(void)
{
	size_t total;

	pthread_mutex_lock(&g_malloc.lock);

	total = 0;
	total += show_zones(g_malloc.tiny, "TINY");
	total += show_zones(g_malloc.small, "SMALL");
	total += show_zones(g_malloc.large, "LARGE");

	putstr("Total : ");
	putnbr(total);
	putstr(" byte(s)\n");

	pthread_mutex_unlock(&g_malloc.lock);
}
