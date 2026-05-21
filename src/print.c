#include "malloc.h"

#include <stdio.h> // printf()

void show_alloc_mem_ex(void)
{
	printf("---DATA---\n");
	printf("sizeof(t_zone) : %zu\nsizepf(t_chunk) : %zu\n", sizeof(t_zone), sizeof(t_chunk));
	printf("TINY : %p\n", g_zone_tiny);
	while (g_zone_tiny) {
		printf("---ZONE---\n");
		printf("address : %p\nsize : %zu\n", g_zone_tiny, g_zone_tiny->total_size);
		while (g_zone_tiny->chunks) {
			printf("---CHUNK---\n");
			printf("address : %p\nsize : %zu\n", g_zone_tiny->chunks, g_zone_tiny->chunks->size);
			g_zone_tiny->chunks = g_zone_tiny->chunks->next;
		}
		g_zone_tiny = g_zone_tiny->next;
	}

	printf("SMALL : %p\n", g_zone_small);
	printf("LARGE : %p\n", g_zone_large);
}
