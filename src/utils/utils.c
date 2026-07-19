#include <unistd.h>

void putstr(char *str)
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

void puthex(size_t nb)
{
	if (nb >= 16)
		puthex(nb / 16);
	write("")
}

int main()
{
	write(1, "\n", 1);
}
