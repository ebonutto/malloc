#include "malloc_int.h"

#include <stddef.h> // size_t

static void puthex_byte(unsigned char byte)
{
	static const char hex[] = "0123456789abcdef";

	ft_putchar(hex[byte >> 4]);
	ft_putchar(hex[byte & 0xf]);
}

void hexdump(const void *ptr, size_t size)
{
	const unsigned char *p;
	size_t i;
	size_t j;
	unsigned char c;

	p = (const unsigned char *)ptr;

	i = 0;
	while (i < size) {
		ft_putstr("      ");
		ft_putptr(p + i);
		ft_putstr("  ");

		j = 0;
		while (j < 16) {
			if (i + j < size) {
				puthex_byte(p[i + j]);
				ft_putchar(' ');
			}
			else
				ft_putstr("   ");
			if (j == 7)
				ft_putchar(' ');
			j++;
		}

		ft_putchar(' ');

		j = 0;
		while (j < 16 && i + j < size) {
			c = p[i + j];
			ft_putchar(c >= 32 && c < 127 ? c : '.');
			j++;
		}

		ft_putchar('\n');
		i += 16;
	}
}
