#include "malloc.h"

#include <stddef.h> // size_t
#include <stdio.h> // printf()

static void show_data(void);
static size_t show_zones(t_zone *g_zone, const char *zone_name);

void show_alloc_mem_ex(void)
{
	size_t total;

	show_data();
	total = 0;
	total += show_zones(g_tiny_zones, "TINY");
	total += show_zones(g_small_zones, "SMALL");
	total += show_zones(g_large_zones, "LARGE");
	printf("Total : %zu bytes\n", total);
}

static void show_data(void)
{
	printf("sizeof(t_zone) : %zu\n", sizeof(t_zone));
	printf("sizeof(t_chunk) : %zu\n", sizeof(t_chunk));
}

static void hexdump(const void *ptr, size_t size)
{
	const unsigned char *p;
	size_t i;
	size_t j;

	p = ptr;
	i = 0;
	for (i = 0; i < size; i += 16) {
		printf("%04zx  ", i);
		for (j = 0; j < 16; j++) {
			if (i + j < size)
				printf("%02x ", p[i + j]);
			else
				printf("   ");
			if (j == 7)
				printf(" ");
		}
		printf(" ");
		for (j = 0; j < 16 && i + j < 16 ; j++)
			printf("%c", (p[i+j] >= 32 && p[i+j] < 127) ? p[i+j] : '.');
		printf("\n");
	}
}

static size_t show_chunks(t_chunk *chunk)
{
	size_t total;
	void *start;
	void *end;

	total = 0;
	while (chunk) {
		start = (void *)(chunk + 1);
		end = (void *)((char *)start + chunk->size);
		printf("%p - %p : %zu bytes | free : %zu", start, end,
			chunk->size, chunk->flags & CHUNK_FREE);
		if (chunk->flags & CHUNK_FREE)
			hexdump(start, chunk->size);
		total += chunk->size;
		chunk = chunk->next;
		break ;
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
