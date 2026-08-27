#include "malloc.h"
#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t

static void show_data(void)
{
	ft_putstr("--- DATA ---\n");

	ft_putstr("  pagesize : ");
	ft_putnbr(get_page_size());
	ft_putstr(" bytes\n");

	ft_putstr("  TINY_MAX : ");
	ft_putnbr(TINY_MAX);
	ft_putstr(" bytes\n");

	ft_putstr("  SMALL_MAX : ");
	ft_putnbr(SMALL_MAX);
	ft_putstr(" bytes\n");

	ft_putstr("  sizeof(t_zone) : ");
	ft_putnbr(sizeof(t_zone));
	ft_putstr(" bytes\n");

	ft_putstr("  sizeof(t_chunk) : ");
	ft_putnbr(sizeof(t_chunk));
	ft_putstr(" bytes\n");
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

		ft_putstr("    --- CHUNK : ");
		ft_putptr(start);
		ft_putstr(" - ");
		ft_putptr(end);
		ft_putstr(" : ");
		ft_putnbr(chunk->size);
		ft_putstr(" bytes (");
		if (chunk->flags & CHUNK_FREE)
			ft_putstr("free");
		else
			ft_putstr("occupied");
		ft_putstr(") ---\n");

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

	ft_putstr("--- ");
	ft_putstr(name);
	ft_putstr(" : ");
	ft_putptr(zone);
	ft_putstr(" ---\n");

	total = 0;
	while (zone) {
		ft_putstr("  --- ZONE : ");
		ft_putptr(zone);
		ft_putstr(" : ");
		ft_putnbr(zone->size);
		ft_putstr(" bytes ---\n");

		total += show_chunks(zone->chunks);
		zone = zone->next;
	}

	return (total);
}

void show_alloc_mem_ex(void)
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

	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr(" byte(s)\n");

	pthread_mutex_unlock(&g_malloc.lock);
}
