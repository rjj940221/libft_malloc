#include "libft_Malloc.h"

t_zones g_zones;

void init()
{
	int i;

	g_zones.tiny_space = mmap(0, (size_t) TINY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	i = -1;
	while (++i < TINY_BLOCKS)
		g_zones.tiny[i] = (t_block) {0, TRUE, g_zones.tiny_space + (TINY_SIZE * i)};
	g_zones.small_space = mmap(0, (size_t) SMALL_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	i = -1;
	while (++i < SMALL_BLOCKS)
		g_zones.small[i] = (t_block) {0, TRUE, g_zones.small_space + (SMALL_SIZE * i)};
	g_zones.init = 1;
}

void *allocate_large(size_t size)
{
	void *re;
	t_zone_link *zone;

	re = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	zone = (t_zone_link *) mmap(0, sizeof(t_zone_link), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (re != MAP_FAILED && zone != MAP_FAILED) {
		zone->location = re;
		zone->size = size;
		zone->previous = NULL;
		zone->next = NULL;
		if (g_zones.large) {
			zone->next = g_zones.large;
			g_zones.large->previous = zone;
		}
		g_zones.large = zone;
		return re;
	}
	return NULL;
}

void *allocate_small(size_t size)
{
	int i;

	i = 0;
	if (g_zones.small_space == MAP_FAILED)
		return allocate_large(size);
	while (i < SMALL_BLOCKS) {
		if (g_zones.small[i].free == TRUE) {
			g_zones.small[i].free = FALSE;
			g_zones.small[i].size = size;
			return g_zones.small[i].location;
		}
		i++;
	}
	return allocate_large(size);
}

void *allocate_tiny(size_t size)
{
	int i;

	i = 0;
	if (g_zones.tiny_space == MAP_FAILED)
		return allocate_large(size);
	while (i < TINY_BLOCKS) {
		if (g_zones.tiny[i].free == TRUE) {
			g_zones.tiny[i].free = FALSE;
			g_zones.tiny[i].size = size;
			return g_zones.tiny[i].location;
		}
		i++;
	}
	return allocate_small(size);
}

void *malloc(size_t size)
{
	if (g_zones.init == 0)
		init();
	if (size == 0)
		return NULL;
	if (size <= TINY_SIZE)
		return allocate_tiny(size);
	if (size <= SMALL_BLOCKS)
		return allocate_small(size);
	return allocate_large(size);
};
