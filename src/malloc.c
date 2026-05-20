#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

s_zone *create_tiny_zone(size_t size)
{
	s_zone *zone;

	zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
		return (perror("mmap()"), NULL);

	zone->chunk = 

	return (zone);
}

void *allocate_tiny(size_t size)
{
	s_zone *zone;
	s_chunk *chunk;

	zone = create_tiny_zone();

	chunk = find_free_chunk()
}

void *malloc(size_t size)
{
	if (size <= TINY_MAX)
		return allocate_tiny(size);
	//if (size <= SMALL_MAX)
	//	return allocate_small(size);
	//return allocate_large(size);
}
