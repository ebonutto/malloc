#include "malloc.h"

#include <sys/mman.h> // MAP_ANONYMOUS, MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()

#include <stddef.h> // size_t, NULL
#include <stdio.h> // perror()
#include <unistd.h> // getpagesize()

void *malloc(size_t size)
{
	if (size <= TINY_MAX)
		return allocate_tiny(size);
	else if (size <= SMALL_MAX)
		return allocate_small(size);
	return allocate_large(size);
}

void *allocate_tiny(size_t size)
{
	t_zone *zone;
	t_chunk *chunk;

	zone = find_tiny_zone(size);
	if (!zone)
		zone = create_tiny_zone();

	chunk = find_free_chunk()
}
