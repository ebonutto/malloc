#include "malloc.h"

#include <stddef.h> // size_t
#include <stdio.h> // printf()

static size_t show_zone(t_zone *g_zone, const char *zone_name);

void show_alloc_mem(void)
{
	size_t total;

	total = 0;
	total += show_zone(g_zone_tiny, "TINY");
	total += show_zone(g_zone_small, "SMALL");
	total += show_zone(g_zone_large, "LARGE");

	printf("Total : %zu bytes\n", total);
}

static size_t show_chunk(t_chunk *chunk)
{
	size_t start_addr;
	size_t end_addr;
	size_t total;

	total = 0;
	while (chunk) {
		if (chunk->free == 0) {
			start_addr = (size_t)(chunk + 1);
			end_addr = start_addr + chunk->size;
			printf("%p - %p : %zu bytes\n",
				(void *)start_addr,
				(void *)end_addr,
				chunk->size);
			total += chunk->size;
		}
		chunk = chunk->next;
	}

	return (total);
}

static size_t show_zone(t_zone *g_zone, const char *zone_name)
{
	t_zone *zone;
	size_t total;

	printf("%s : %p\n", zone_name, (void *)g_zone);

	total = 0;
	zone = g_zone;
	while (zone) {
		total += show_chunk(zone->chunks);
		zone = zone->next;
	}

	return (total);
}
