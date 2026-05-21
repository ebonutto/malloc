#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_zone *g_zone_tiny = NULL;
t_zone *g_zone_small = NULL;
t_zone *g_zone_large = NULL;

void *alloc_chunk(t_zone *head, size_t size)
{
	t_chunk *chunk = (t_chunk *)((char *)head + sizeof(t_zone));

	chunk->size = size;
	chunk->next = head->chunks;
	chunk->free = 0;

	head->chunks = chunk;
	return ((void *)((char *)chunk + sizeof(t_chunk)));
}

void *alloc_in_zone(t_zone **head, size_t size)
{
	t_zone *zone;
	//t_chunk *chunk;

	//zone = *head;
	//while (zone) {
	//	chunk = find_free_space();
	//	zone = zone->next;
	//}
	zone = create_zone(size);
	if (!zone)
		return (NULL);
	zone->next = *head;
	*head = zone;
	return (alloc_chunk(*head, size));
}

void *malloc(size_t size)
{
	return (alloc_in_zone(&g_zone_tiny, size));
	return (NULL);
}
