#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()
#include <stddef.h> // size_t, NULL
#include <stdint.h> // SIZE_MAX
#include <strings.h> // bzero()

void *calloc(size_t count, size_t size)
{
	void *ptr;
	size_t total;

	pthread_mutex_lock(&g_malloc.lock);

	if (count != 0 && size > SIZE_MAX / count) {
		if (g_malloc.flags & MALLOC_HISTORY)
			history_push(LOG_CALLOC, NULL, NULL, 0);

		pthread_mutex_unlock(&g_malloc.lock);
		return (NULL);
	}

	total = count * size;

	ptr = malloc_impl(total);
	if (ptr)
		bzero(ptr, total);

	if (g_malloc.flags & MALLOC_HISTORY)
			history_push(LOG_CALLOC, ptr, NULL, total);

	pthread_mutex_unlock(&g_malloc.lock);
	return (ptr);
}
