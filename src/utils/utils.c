#include "malloc_int.h"

#include <unistd.h> // write()

#include <stddef.h> // size_t, uintptr_t

void putchar(int c)
{
	unsigned char ch;

	ch = c;
	write(1, &ch, 1);
}

void putstr(const char *str)
{
	size_t count;

	count = 0;
	while (str[count])
		count++;
	write(1, str, count);
}

void putnbr(size_t nb)
{
	if (nb >= 10)
		putnbr(nb / 10);
	putchar('0' + nb % 10);
}

static void puthex(uintptr_t nb)
{
	static const char hex[] = "0123456789abcdef";

	if (nb >= 16)
		puthex(nb / 16);
	putchar(hex[nb % 16]);
}

void putptr(const void *ptr)
{
	putstr("0x");
	puthex((uintptr_t)ptr);
}
