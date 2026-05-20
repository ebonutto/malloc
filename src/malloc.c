#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_zone *create_zone(size_t size)
{
	t_zone *zone;

	zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (perror("mmap()"), NULL);

	zone->chunk = 

	return (zone);
}

void *alloc_in_zone(t_zone **head, size_t size, size_t zone_size)
{
	t_zone *zone;
	t_chunk *chunk;

	zone = *head;
	//while (zone) {
	//	chunk = find_free_chunk();
	//	if (chunk)
	//		return ;
	//	zone = zone->next;
	//}
	zone = create_zone(size, );
	zone->next = *head;
	*head = zone;
	return ;
}

void *malloc(size_t size)
{
	if (size <= TINY_MAX)
		return allocate_tiny(size);
	//if (size <= SMALL_MAX)
		//return allocate_small(size);
	//return allocate_large(size);
}
