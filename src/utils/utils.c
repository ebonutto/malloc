#include "malloc_int.h"

#include <stddef.h> // size_t
#include <stdint.h> // uintptr_t
#include <unistd.h> // write()

void *ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	while (n--)
		*d++ = *s++;

	return (dest);
}

void *ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char *d = dst;
	const unsigned char *s = src;

	if (d == s)
		return (dst);

	if (d < s) {
		while (len--)
			*d++ = *s++;
	} else {
		d += len;
		s += len;
		while (len--)
			*--d = *--s;
	}

	return (dst);
}

void *ft_memset(void *b, int c, size_t len)
{
	unsigned char *d = b;

	while (len--)
		*d++ = c;

	return (b);
}

void ft_putchar(int c)
{
	unsigned char ch = c;

	write(1, &ch, 1);
}

void ft_putstr(const char *str)
{
	size_t count = 0;

	while (str[count])
		count++;

	write(1, str, count);
}

void ft_putnbr(size_t nb)
{
	if (nb >= 10)
		ft_putnbr(nb / 10);
	ft_putchar('0' + nb % 10);
}

static void ft_puthex(uintptr_t nb)
{
	static const char hex[] = "0123456789abcdef";

	if (nb >= 16)
		ft_puthex(nb / 16);
	ft_putchar(hex[nb % 16]);
}

void ft_putptr(const void *ptr)
{
	ft_putstr("0x");
	ft_puthex((uintptr_t)ptr);
}
