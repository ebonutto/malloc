#include "malloc_int.h"

#include <stddef.h> // NULL
#include <stdlib.h> // getenv()

void init_env(void)
{
	static int initialized = 0;
	char *value;

	if (initialized)
		return ;
	value = getenv("MALLOC_HISTORY");
	if (value)
		g_malloc.flags |= MALLOC_HISTORY;
	value = getenv("MALLOC_SCRIBBLE");
	if (value)
		g_malloc.flags |= MALLOC_SCRIBBLE;
	initialized = 1;
}
