#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()

static void show_log(t_history *history, size_t total, size_t start)
{
	// static const char *op_str[] = {
	// 	[LOG_MALLOC] = "MALLOC",
	// 	[LOG_FREE] = "FREE",
	// 	[LOG_REALLOC] = "REALLOC",
	// 	[LOG_CALLOC] = "CALLOC",
	// };
	t_log *log;
	size_t i;

	i = 0;
	while (i < total) {
		log = &history->logs[(start + i) & (HISTORY_SIZE - 1)];
		i++;
		// printf("#%04zu [%s] %p", i, op_str[log->op], log->ptr);
		// if (log->op == LOG_REALLOC)
		// 	printf(" -> %p", log->new_ptr);
		// if (log->op != LOG_FREE)
		// 	printf(" %zu", log->size);
		// printf("\n");
	}
	(void)log;
}

void show_history(void)
{
	t_history *history;
	size_t count;
	size_t total;
	size_t start;

	history = &g_malloc.history;
	count = history->count;
	if (count == 0)
		return ;
	// printf("--- HISTORY (%zu ops", count);
	// if (count > HISTORY_SIZE)
	// 	printf(", showing last %d", HISTORY_SIZE);
	// printf(") ---\n");
	total = count < HISTORY_SIZE ? count : HISTORY_SIZE;
	start = count < HISTORY_SIZE ? 0 : count & (HISTORY_SIZE - 1);
	show_log(history, total, start);
}
