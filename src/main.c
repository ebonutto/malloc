#include "malloc.h"

#include <stdio.h> // printf()

int main()
{
	const char *hello = "Hello World!";
	char *str;
	int i = 0;

	str = malloc(12);
	i = 0;
	while (hello[i]) {
		str[i] = hello[i];
		i++;
	}
	str[i] = '\0';


	show_alloc_mem_ex();

	free(str);
	show_alloc_mem_ex();
	return (0);
}
