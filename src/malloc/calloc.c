#include "malloc_int.h"

#include <stddef.h> // NULL
#include <stdint.h> // SIZE_MAX
#include <strings.h> // bzero()

void *calloc(size_t count, size_t size)
{
	void *ptr;

	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	size = count * size;
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	bzero(ptr, size);
	return (ptr);
}
