#include "malloc_int.h"

void history_push(t_log_op op, void *ptr, size_t size)
{
	t_history *history;
	t_log *log;

	history = &g_malloc.history;
	log = &history->logs[history->count & (HISTORY_SIZE - 1)];
	log->op = op;
	log->ptr = ptr;
	log->size = size;
	history->count++;
}
