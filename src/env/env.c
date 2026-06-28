#include "malloc_int.h"

#include <stddef.h> // NULLw
#include <stdlib.h> // getenv()

int init_env(void)
{
	static int initialized = 0;
	char *value;

	if (initialized)
		return (0);
	value = getenv("MALLOC_LOG");
	if (value)
		g_malloc.flags |= MALLOC_LOG;
	return (0);
}
