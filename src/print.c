#include "malloc.h"

#include <stdio.h> // printf()

void    show_alloc_mem_ex(void)
{
	t_zone  *zone;
	t_chunk *chunk;

	printf("---DATA---\n");
	printf("sizeof(t_zone)  : %zu\n", sizeof(t_zone));
	printf("sizeof(t_chunk) : %zu\n", sizeof(t_chunk));
	printf("TINY : %p\n", g_zone_tiny);

	zone = g_zone_tiny;  // ✅ copie locale, jamais toucher le global
	while (zone)
	{
		printf("---ZONE---\n");
		printf("address : %p\n", zone);
		printf("size    : %zu\n", zone->total_size);

		chunk = zone->chunks;  // ✅ copie locale
		while (chunk)
		{
			printf("---CHUNK---\n");
			printf("address : %p\n", chunk);
			printf("size    : %zu\n", chunk->size);
			printf("free    : %d\n",  chunk->free);
			chunk = chunk->next;  // avance la copie, pas zone->chunks
		}
		zone = zone->next;  // avance la copie, pas g_zone_tiny
	}

	printf("SMALL : %p\n", g_zone_small);
	printf("LARGE : %p\n", g_zone_large);
}