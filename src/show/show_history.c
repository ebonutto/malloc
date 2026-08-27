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

		ft_putchar('#');
		ft_putnbr(i);

		ft_putstr("  ");

		ft_putstr(op_str[log->op]);

		ft_putstr("  ");

		ft_putptr(log->ptr);

		if (log->op != LOG_FREE) {
			if (log->op == LOG_REALLOC) {
				ft_putstr(" -> ");
				ft_putptr(log->new_ptr);
			}

			ft_putstr("  ");

			ft_putnbr(log->size);
			ft_putstr(" byte(s)");
		}

		ft_putchar('\n');
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

	ft_putstr("-- HISTORY (");
	ft_putnbr(count);
	ft_putstr(" log(s)");
	if (count > HISTORY_SIZE) {
		ft_putstr(", showing last ");
		ft_putnbr(HISTORY_SIZE);
	}
	ft_putstr(") ---\n");

	total = count < HISTORY_SIZE ? count : HISTORY_SIZE;
	start = count < HISTORY_SIZE ? 0 : count & (HISTORY_SIZE - 1);
	show_logs(history, total, start);
}
