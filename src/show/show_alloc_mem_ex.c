#include "malloc_int.h"

#include <pthread.h> // PTHREAD_MUTEX_INITIALIZER, pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t
#include <stdio.h> // printf()

static void show_data(void)
{
	printf("--- DATA ---\n");
	printf("  sizeof(t_zone) : %zu bytes\n", sizeof(t_zone));
	printf("  sizeof(t_chunk) : %zu bytes\n", sizeof(t_chunk));
}

static void hexdump(const void *ptr, size_t size)
{
	const unsigned char *p;
	size_t i;
	size_t j;

	p = ptr;
	i = 0;
	for (i = 0; i < size; i += 16) {
		printf("      %04zx  ", i);
		for (j = 0; j < 16; j++) {
			if (i + j < size)
				printf("%02x ", p[i + j]);
			else
				printf("   ");
			if (j == 7)
				printf(" ");
		}
		printf(" ");
		for (j = 0; j < 16 && i + j < size ; j++)
			printf("%c",
			       (p[i+j] >= 32 && p[i+j] < 127) ? p[i+j] : '.');
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
		printf("    --- CHUNK : %p - %p : %zu bytes (free : %zu)\n",
		       start, end, chunk->size, chunk->flags & CHUNK_FREE);
		if (!(chunk->flags & CHUNK_FREE) && (g_malloc.flags & MALLOC_HEXDUMP))
			hexdump(start, chunk->size);
		total += chunk->size;
		chunk = chunk->next;
	}
	return (total);
}

static size_t show_zones(t_zone *g_zone, const char *zone_name)
{
	t_zone *zone;
	size_t total;

	printf("--- %s : %p ---\n", zone_name, (void *)g_zone);
	zone = g_zone;
	total = 0;
	while (zone) {
		printf("  --- ZONE : %p : %zu bytes ---\n",
		       (void *)zone, zone->size);
		total += show_chunks(zone->chunks);
		zone = zone->next;
	}
	return (total);
}

static void show_alloc_mem_ex_impl(void)
{
	size_t total;

	show_data();
	total = 0;
	total += show_zones(g_malloc.tiny, "TINY");
	total += show_zones(g_malloc.small, "SMALL");
	total += show_zones(g_malloc.large, "LARGE");
	if (g_malloc.flags & MALLOC_HISTORY)
		show_history();
	printf("Total : %zu %s\n", total, total > 1 ? "bytes" : "byte");
}

void show_alloc_mem_ex(void)
{
	pthread_mutex_lock(&g_malloc.lock);
	show_alloc_mem_ex_impl();
	pthread_mutex_unlock(&g_malloc.lock);
}
