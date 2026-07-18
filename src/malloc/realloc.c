#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t, NULL
#include <string.h> // memcpy()

static void *realloc_impl(void *ptr, size_t size)
{
	t_chunk *chunk;
	void *new;

	if (!ptr)
		return (malloc_impl(size));
	if (size == 0)
		return (free_impl(ptr), NULL);
	size = ALIGN16(size);
	chunk = (t_chunk *)((char *)ptr - CHUNK_HEADER);
	if (chunk->size >= size)
		return (ptr);
	//TODO ADD LEFTOVER CASE
	new = malloc_impl(size);
	memcpy(new, ptr, chunk->size);
	free_impl(ptr);
	return (new);
}

void *realloc(void *ptr, size_t size)
{
	void *new_ptr;

	pthread_mutex_lock(&g_malloc.lock);
	new_ptr = realloc_impl(ptr, size);
	if (g_malloc.flags & MALLOC_HISTORY)
		history_push(LOG_REALLOC, ptr, new_ptr, size);
	pthread_mutex_unlock(&g_malloc.lock);
	return (new_ptr);
}
