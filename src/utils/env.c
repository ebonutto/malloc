#include "malloc_int.h"

#include <stdlib.h> // getenv()

__attribute__((constructor))
static void init_env(void)
{
	if (getenv("MALLOC_DATA"))
		g_malloc.flags |= MALLOC_DATA;
	if (getenv("MALLOC_HEXDUMP"))
		g_malloc.flags |= MALLOC_HEXDUMP;
	if (getenv("MALLOC_HISTORY"))
		g_malloc.flags |= MALLOC_HISTORY;
}
