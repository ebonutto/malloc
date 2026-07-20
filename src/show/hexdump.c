#include "malloc_int.h"

#include <stddef.h> // size_t

static void puthex_fixed(size_t nb, size_t width)
{
	static const char hex[] = "0123456789abcdef";
	char buf[32];
	size_t len;

	len = 0;
	while (nb) {
		buf[len++] = hex[nb & 0xf];
		nb >>= 4;
	}
	while (len < width)
		buf[len++] = '0';
	while (len)
		putchar(buf[--len]);
}

static void puthex_byte(unsigned char byte)
{
	static const char hex[] = "0123456789abcdef";

	putchar(hex[byte >> 4]);
	putchar(hex[byte & 0xf]);
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
		putstr("      ");
		puthex_fixed(i, 4);
		putstr("  ");

		j = 0;
		while (j < 16) {
			if (i + j < size) {
				puthex_byte(p[i + j]);
				putchar(' ');
			}
			else
				putstr("   ");
			if (j == 7)
				putchar(' ');
			j++;
		}

		putchar(' ');

		j = 0;
		while (j < 16 && i + j < size) {
			c = p[i + j];
			putchar(c >= 32 && c < 127 ? c : '.');
			j++;
		}
		putchar('\n');
		i += 16;
	}
}
