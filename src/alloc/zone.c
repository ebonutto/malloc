#include "malloc_int.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <unistd.h> // _SC_PAGESIZE, sysconf()

size_t get_page_size(void)
{
	static size_t page = 0;

	if (!page)
		page = sysconf(_SC_PAGESIZE);
	return (page);
}

t_zone *create_zone(size_t zone_size, size_t chunk_type)
{
	t_zone *zone;

	zone_size = ALIGN_PAGE(zone_size, get_page_size());

	zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
	            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (NULL);

	zone->size = zone_size;
	zone->prev = NULL;
	zone->next = NULL;
	zone->chunks = create_chunk(zone, chunk_type);

	return (zone);
}

void zone_prepend(t_zone **head, t_zone *zone)
{
	zone->next = *head;
	if (zone->next)
		zone->next->prev = zone;
	*head = zone;
}
