#include "malloc_int.h"

#include <stdio.h> // printf()


static void show_log(t_history *history, size_t total, size_t start)
{
	static const char *op_str[] = {
		[LOG_MALLOC] = "MALLOC",
		[LOG_FREE] = "FREE",
		[LOG_REALLOC] = "REALLOC",
		[LOG_CALLOC] = "CALLOC",
	};

	t_log *log;
	size_t i;

	i = 0;
	while (i < total) {
		log = &history->logs[(start + i) & (HISTORY_SIZE - 1)];
		printf("#%04zu [%s] TINY %p %zu\n",
		       i, op_str[log->op], log->ptr, log->size);
		i++;
	}
}

void show_history()
{
	t_history *history;
	size_t count;
	size_t total;
	size_t start;

	history = &g_malloc.history;
	count = history->count;
	if (count == 0)
		return ;
	printf("--- HISTORY ---\n");
	total = count < HISTORY_SIZE ? count : HISTORY_SIZE;
	start = count < HISTORY_SIZE ? 0 : count & (HISTORY_SIZE - 1);
	show_log(history, total, start);
}
