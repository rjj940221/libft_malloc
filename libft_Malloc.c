#include "libft_Malloc.h"

t_zones g_zones;

void init()
{
	int i;

	if (!g_zones.small_space) {
		g_zones.small_space = mmap(0, (size_t) SMALL_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		i = -1;
		while (++i < SMALL_BLOCKS)
			g_zones.small[i] = {0, (size_t) (SMALL_SIZE), TRUE, g_zones.small_space + (SMALL_SIZE * i)};
	}
	if (!g_zones.tiny_space) {
		g_zones.tiny_space = mmap(0, (size_t) TINY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		i = 0;
		while (++i < TINY_BLOCKS)
			g_zones.tiny[i] = {0, (size_t) (TINY_SIZE), TRUE, g_zones.tiny_space + (TINY_SIZE * i)};
	}
	g_zones.init = 1;
}

t_zone_link *find_zone(void *adr)
{
	t_zone_link *iter;

	iter = g_zones.large;
	while (iter != NULL) {
		if (iter->location == adr)
			return (iter);
	}
	return NULL;
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
		if (g_zones.large)
			zone->next = g_zones.large;
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

void *realloc_large(void *ptr, size_t size)
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

void *realloc_small(unsigned long index, size_t size)
{
	void *re;

	if (size <= SMALL_SIZE && size > TINY_SIZE) {
		g_zones.tiny[index].size = size;
		return g_zones.tiny[index].location;
	}
	else{
		re = malloc(size);
		g_zones.tiny[index].free = TRUE;
		memcpy(re, g_zones.tiny[index].location, (g_zones.tiny[index].size < size) ? g_zones.tiny[index].size : size);
		return re;
	}
}
void *realloc_tiny(unsigned long index, size_t size)
{
	void *re;

	if (size <= TINY_SIZE) {
		g_zones.tiny[index].size = size;
		return g_zones.tiny[index].location;
	}
	else{
		re = malloc(size);
		g_zones.tiny[index].free = TRUE;
		memcpy(re, g_zones.tiny[index].location, (g_zones.tiny[index].size < size) ? g_zones.tiny[index].size : size);
		return re;
	}
}

void *realloc(void *ptr, size_t size)
{
	if (ptr >= g_zones.tiny_space && ptr <= g_zones.tiny_space + (TINY_SIZE * TINY_BLOCKS)) {
		if (((unsigned long) (ptr) % (unsigned long) g_zones.tiny_space) % TINY_SIZE == 0) {
			realloc_tiny(((unsigned long) (ptr) % (unsigned long) g_zones.tiny_space) / TINY_SIZE, size);
		} else {
			//todo: invalid access
		}
	}
	if (ptr >= g_zones.small_space && ptr <= g_zones.small_space + (SMALL_SIZE * SMALL_BLOCKS)) {
		if (((unsigned long) (ptr) % (unsigned long) g_zones.small_space) % SMALL_SIZE == 0) {


		} else {
			//todo: invalid access
		}
	}
	return realloc_large(ptr, size);
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