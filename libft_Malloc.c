#include "libft_Malloc.h"

t_zone_link *g_allocated_zones = NULL;

t_zone_link *find_zone(void *adr)
{
	t_zone_link *iter;

	iter = g_allocated_zones;
	while (iter != NULL) {
		if (iter->location == adr)
			return (iter);
	}
	return NULL;
}

void *malloc(size_t size)
{
	void *re;
	t_zone_link *zone;

	re = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	zone = (t_zone_link *) mmap(0, sizeof(t_zone_link), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (re != MAP_FAILED && zone != MAP_FAILED) {
		zone->location = re;
		zone->size = size;
		if (g_allocated_zones)
			zone->next = g_allocated_zones;
		g_allocated_zones = zone;
		return re;
	}
	return NULL;
};

void *realloc(void *ptr, size_t size)
{
	t_zone_link *old;
	void *re;

	old = find_zone(ptr);
	if (!old) {
		//todo: throw error
	} else {
		re = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (re != MAP_FAILED) {
			memcpy(re, old->location, (size < old->size) ? size : old->size);
			if (munmap(ptr, old->size)) {
				//todo: throw error
				return NULL;
			}
			old->location = re;
			old->size = size;
			return re;
		}
	}
	return NULL;
}

void free(void *ptr)
{
	t_zone_link *old;

	old = find_zone(ptr);
	if (!old) {
		//todo: throw error
	}
	if (munmap(old->location, old->size) == -1) {
		//todo: throw error
	}
}