#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t

static void show_data(void)
{
	putstr("--- DATA ---\n");

	putstr("  pagesize : ");
	putnbr(get_page_size());
	putstr(" bytes\n");

	putstr("  TINY_MAX : ");
	putnbr(TINY_MAX);
	putstr(" bytes\n");

	putstr("  SMALL_MAX : ");
	putnbr(SMALL_MAX);
	putstr(" bytes\n");

	putstr("  sizeof(t_zone) : ");
	putnbr(sizeof(t_zone));
	putstr(" bytes\n");

	putstr("  sizeof(t_chunk) : ");
	putnbr(sizeof(t_chunk));
	putstr(" bytes\n");
}

static size_t show_chunks(t_chunk *chunk)
{
	size_t total;
	void *start;
	void *end;

	total = 0;
	while (chunk) {
		start = (char *)chunk + sizeof(t_chunk);
		end = (char *)start + chunk->size;

		putstr("    --- CHUNK : ");
		putptr(start);
		putstr(" - ");
		putptr(end);
		putstr(" : ");
		putnbr(chunk->size);
		putstr(" bytes (");
		if (chunk->flags & CHUNK_FREE)
			putstr("free");
		else
			putstr("occupied");
		putstr(") ---\n");

		if (!(chunk->flags & CHUNK_FREE) && (g_malloc.flags & MALLOC_HEXDUMP))
			hexdump(start, chunk->size);

		total += chunk->size;
		chunk = chunk->next;
	}

	return (total);
}

static size_t show_zones(t_zone *zone, const char *name)
{
	size_t total;

	putstr("--- ");
	putstr(name);
	putstr(" : ");
	putptr(zone);
	putstr(" ---\n");

	total = 0;
	while (zone) {
		putstr("  --- ZONE : ");
		putptr(zone);
		putstr(" : ");
		putnbr(zone->size);
		putstr(" bytes ---\n");

		total += show_chunks(zone->chunks);
		zone = zone->next;
	}

	return (total);
}

static void show_alloc_mem_ex(void)
{
	size_t total;

	pthread_mutex_lock(&g_malloc.lock);

	if (g_malloc.flags & MALLOC_DATA)
		show_data();

	total = 0;
	total += show_zones(g_malloc.tiny, "TINY");
	total += show_zones(g_malloc.small, "SMALL");
	total += show_zones(g_malloc.large, "LARGE");

	if (g_malloc.flags & MALLOC_HISTORY)
		show_history();

	putstr("Total : ");
	putnbr(total);
	putstr(" byte(s)\n");

	pthread_mutex_unlock(&g_malloc.lock);
}
