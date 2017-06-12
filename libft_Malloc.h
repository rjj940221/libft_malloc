#ifndef UNTITLED_LIBRARY_H
# define UNTITLED_LIBRARY_H
# define TINY_SIZE getpagesize()
# define TINY_BLOCKS 100
# define SMALL_SIZE 4 * getpagesize()
# define SMALL_BLOCKS 100
#define FALSE 0
#define TRUE 1
# include <stdlib.h>
# include <sys/mman.h>
# include "libft/libft.h"

typedef struct s_block{
	size_t  size;
	size_t  max_size;
	int     free;
	void    *location;
} t_block;

typedef struct s_zone_link
{
	size_t size;
	void *location;
	struct s_zone_link* next;
}t_zone_link;

typedef struct s_zones{
	t_block tiny[TINY_BLOCKS];
	void    *tiny_space;
	t_block small[SMALL_BLOCKS];
	void    *small_space;

} t_zones;

t_zone_link *g_allocated_zones;

void free(void *ptr);

void *malloc(size_t size);

void *realloc(void *ptr, size_t size);

void show_alloc_mem();


#endif