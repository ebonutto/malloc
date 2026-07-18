#include "malloc_int.h"

#include <stdlib.h> // getenv()

__attribute__((constructor))
void init_env(void)
{
	if (getenv("MALLOC_HISTORY"))
		g_malloc.flags |= MALLOC_HISTORY;
	if (getenv("MALLOC_HEXDUMP"))
		g_malloc.flags |= MALLOC_HEXDUMP;
}
