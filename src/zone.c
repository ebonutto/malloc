#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

t_zone *create_zone(size_t size)
{
	t_zone *zone;
	t_chunk *chunk;
	size_t page;

	page = getpagesize();
	size = (size + page - 1) & ~(page - 1);

	zone = mmap(NULL, size, PROT_READ | PROT_WRITE,
	            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (perror("mmap()"), NULL);

	zone->total_size = size;
	zone->next = NULL;

	chunk = (t_chunk *)((char *)zone + sizeof(t_zone));
	chunk->size = zone->total_size - sizeof(t_zone) - sizeof(t_chunk);
	chunk->free = 1;
	chunk->next = NULL;
	chunk->prev = NULL;

	zone->chunks = chunk;
	return (zone);
}
