#include "malloc_int.h"

#include <stddef.h> // NULL
#include <stdlib.h> // getenv()
#include <stdio.h>
int init_env(void)
{
	static int initialized = 0;
	char *value;

	if (initialized)
		return (0);
	value = getenv("MALLOC_HISTORY");
	if (value)
		g_malloc.flags |= MALLOC_HISTORY;
	initialized = 1;
	return (0);
}
