#include "malloc.h"

#include <stdio.h> // printf()

void show_alloc_mem_ex(void)
{
	printf("TINY : %p\n", g_zone_tiny);
	printf("SMALL : %p\n", g_zone_small);
	printf("LARGE : %p\n", g_zone_large);
}
