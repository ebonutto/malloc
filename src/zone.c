#include "malloc.h"

s_zone *find_tiny_zone(size_t size)
{
	s_zone *zone = g_tiny_zones;

	while (zone) {
		if (has_free_space(z, size))
			return z;
		zone = zone->next;
	}

	return (NULL);
}
