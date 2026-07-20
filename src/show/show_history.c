#include "malloc_int.h"

#include <pthread.h> // pthread_mutex_lock(), pthread_mutex_unlock()

static const char *log_str(t_log_op op)
{
	if (op == LOG_MALLOC)
		return ("MALLOC");
	if (op == LOG_FREE)
		return ("FREE");
	if (op == LOG_REALLOC)
		return ("REALLOC");
	if (op == LOG_CALLOC)
		return ("CALLOC");
	return ("UNKNOWN");
}

static void show_logs(t_history *history, size_t total, size_t start)
{
	t_log *log;
	size_t i;

	i = 0;
	while (i < total) {
		log = &history->logs[(start + i) & (HISTORY_SIZE - 1)];

		putchar('#');
		putnbr(i + 1);

		putstr(" [");
		putstr(log_str(log->op));
		putstr("] ");

		putptr(log->ptr);

		if (log->op == LOG_REALLOC) {
			putstr(" -> ");
			putptr(log->new_ptr);
		}

		if (log->op != LOG_FREE) {
			putchar(' ');
			putnbr(log->size);
		}

		putchar('\n');
		i++;

		// printf("#%04zu [%s] %p", i, op_str[log->op], log->ptr);
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

	putstr("-- HISTORY (");
	putnbr(count);
	putstr(" log(s)");
	if (count > HISTORY_SIZE) {
		putstr(", showing last ");
		putnbr(HISTORY_SIZE);
	}
	putstr(") ---\n");

	total = count < HISTORY_SIZE ? count : HISTORY_SIZE;
	start = count < HISTORY_SIZE ? 0 : count & (HISTORY_SIZE - 1);
	show_logs(history, total, start);
}
