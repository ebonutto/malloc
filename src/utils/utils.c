#include "malloc_int.h"

#include <unistd.h> // write()

#include <stdint.h> // uintptr_t

size_t strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void putstr(const char *str)
{
	write(1, str, strlen(str));
}

void putnbr(size_t nb)
{
	char c;

	if (nb >= 10)
		putnbr(nb / 10);
	c = '0' + nb % 10;
	write(1, &c, 1);
}

static void putptr_rec(uintptr_t ptr)
{
	static const char hex[] = "0123456789abcdef";
	char c;

	if (ptr >= 16)
		putptr_rec(ptr / 16);
	c = hex[ptr % 16];
	write(STDOUT_FILENO, &c, 1);
}

void putptr(const void *ptr)
{
	putstr("0x");
	putptr_rec((uintptr_t)ptr);
}
