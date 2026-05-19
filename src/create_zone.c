//#include "malloc.h"

//#include <stddef.h> // size_t
//#include <stdio.h>

//t_zone *create_zone(size_t size)
//{
//	t_zone *zone;
//	t_chunk *chunk;

//	zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
//	if (zone == MAP_FAILED)
//		return (perror("mmap()"), NULL);

//	zone->size = size;
//	zone->next = NULL;

//	chunk = (s_chunk *)(zone + sizeof(s_zone));
//	t_chunk *chunk2 = (s_chunk *)((char *)zone + sizeof(s_zone));
//	print("1 = %p, 2 = %p\n", chunk, chunk2);

//	return (zone);
//}


//t_zone *create_zone(size_t size)
//{
//	t_zone *zone;
//	t_chunk *chunk;

//	zone = mmap(NULL, size, PROT_READ | PROT_WRITE,
//			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
//	if (zone == MAP_FAILED)
//		return (perror("mmap()"), NULL);

//	zone->size = size;
//	zone->next = NULL;

//	// avancer en BYTES, pas en structs
//	chunk = (t_chunk *)((char *)zone + sizeof(t_zone));

//	zone->first = chunk;

//	return zone;
//}