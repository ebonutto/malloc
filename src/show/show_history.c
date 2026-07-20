#include "malloc_int.h"

static void show_logs(t_history *history, size_t total, size_t start)
{
	static const char *const op_str[] = {
		[LOG_MALLOC] = "MALLOC",
		[LOG_FREE] = "FREE",
		[LOG_REALLOC] = "REALLOC",
		[LOG_CALLOC] = "CALLOC"
	};
	size_t i;
	t_log *log;

	i = 0;
	while (i < total) {
		log = &history->logs[(start + i) & (HISTORY_SIZE - 1)];

		putchar('#');
		putnbr(i);

		putstr("  ");

		putstr(op_str[log->op]);

		putstr("  ");

		putptr(log->ptr);

		if (log->op != LOG_FREE) {
			if (log->op == LOG_REALLOC) {
				putstr(" -> ");
				putptr(log->new_ptr);
			}

			putstr("  ");

			putnbr(log->size);
			putstr(" byte(s)");
		}

		putchar('\n');
		i++;
	}
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
