#include "malloc.h"

void free(void *ptr)
{
	t_chunk *chunk;

	if (!ptr)
		return ;

	chunk = (t_chunk *)((char *)ptr - sizeof(t_chunk));
	if (chunk->free)
		return ;

	chunk->free = 1;
	
}
