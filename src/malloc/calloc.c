#include "malloc_int.h"

#include <stddef.h> // NULL
#include <stdint.h> // SIZE_MAX
#include <strings.h> // bzero()

static void *calloc_impl(size_t count, size_t size)
{
	void *ptr;

	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	size = count * size;
	ptr = malloc_impl(size);
	if (!ptr)
		return (NULL);
	bzero(ptr, size);
	return (ptr);
}

void *calloc(size_t count, size_t size)
{
	void *ptr;

	init_env();
	pthread_mutex_lock(&g_malloc.lock);
	ptr = calloc_impl(count, size);
	if (g_malloc.flags & MALLOC_HISTORY)
		history_push(LOG_CALLOC, ptr, NULL, count * size); //! count * size can overflow
	pthread_mutex_unlock(&g_malloc.lock);
	return (ptr);
}
