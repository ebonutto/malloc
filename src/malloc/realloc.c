#include "malloc.h"
#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t, NULL

static void *realloc_impl(void *ptr, size_t size)
{
	t_chunk *chunk;
	size_t type;
	void *new;

	if (!ptr)
		return (malloc_impl(size));
	if (size == 0) {
		free_impl(ptr);
		return (NULL);
	}

	size = ALIGN16(size);
	chunk = (t_chunk *)((char *)ptr - CHUNK_HEADER);

	if (size >= chunk->size && size < chunk->size + CHUNK_HEADER)
		return (ptr);

	type = size <= TINY_MAX ? CHUNK_TINY :
	       size <= SMALL_MAX ? CHUNK_SMALL : CHUNK_LARGE;

	if ((chunk->flags & (CHUNK_TINY | CHUNK_SMALL)) == type) { // meme type + taille differente
		coalesce(chunk);
		if (chunk->size >= size) { // chunk->size >= size + CHUNK_HEADER
			new = (char *)chunk + CHUNK_HEADER;
			if (new != ptr)
				ft_memcpy(new, ptr, chunk->size);
			return (alloc_chunk(chunk, size, type));
		}
	}

	new = malloc_impl(size);
	if (!new)
		return (NULL);
	ft_memcpy(new, ptr, chunk->size);
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
