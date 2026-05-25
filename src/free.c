#include "malloc.h"

#include <sys/mman.h> // munmap()

void free(void *ptr)
{
	t_chunk *curr;
	t_chunk *prev;
	t_chunk *next;

	if (!ptr)
		return ;

	curr = (t_chunk *)((char *)ptr - sizeof(t_chunk));
	if (curr->flags & CHUNK_FREE)
		return ;

	curr->flags |= CHUNK_FREE;

	prev = curr->prev;
	next = curr->next;
	if (prev && (prev->flags & CHUNK_FREE)) {
		prev->size += curr->size + sizeof(t_chunk);
		prev->next = next;
		if (next)
			next->prev = prev;
		curr = prev;
	}
	if (next && (next->flags & CHUNK_FREE)) {
		curr->size += next->size + sizeof(t_chunk);
		next = next->next;
		curr->next = next;
		if (next)
			next->prev = curr;
	}

	// t_zone *tiny_zone = g_zone_tiny;
	// if (!tiny_zone)
	// 	return ;

	// while (tiny_zone) {
	// 	curr = tiny_zone->chunks;
	// 	while (curr) {
	// 		if (curr->free == 0) {
				
	// 			return ;
	// 		}
	// 		curr = curr->next;
	// 	}
	// 	tiny_zone = tiny_zone->next;
	// }

	munmap(g_zone_tiny, TINY_ZONE_SIZE);
	g_zone_tiny = NULL;
}
