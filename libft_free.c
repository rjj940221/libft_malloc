

#include <pthread.h>
#include "libft_Malloc.h"

t_zones g_zones;

void free_error(void *ptr)
{
	pid_t id;
	pthread_t thread;
	char name[15];

	id = getpgrp();
	thread = pthread_self();
	pthread_getname_np(thread, name, 15);
	ft_printf("%s(%i,%p) libft_malloc: *** error for object %p: pointer being freed was not allocated\n", name, id,
	          thread, ptr);
	munmap(g_zones.tiny_space, (size_t) (TINY_BLOCKS * TINY_SIZE));
	munmap(g_zones.small_space, (size_t) (SMALL_BLOCKS * SMALL_SIZE));
	pthread_kill(thread, 6);
}

void remove_link(t_zone_link *zone)
{
	if (zone->previous) {
		zone->previous->next = zone->next;
	}
	if (zone->next) {
		zone->next->previous = zone->previous;
	}
}

void free_large(void *ptr)
{
	t_zone_link *old;

	old = find_zone(ptr);
	if (!old) {
		free_error(ptr);
		return;
	}
	remove_link(old);
	munmap(old->location, old->size);
}

void free(void *ptr)
{
	if (ptr == NULL)
		return;
	if (ptr >= g_zones.tiny_space && ptr <= g_zones.tiny_space + (TINY_SIZE * TINY_BLOCKS)) {
		if (((unsigned long) (ptr) % (unsigned long) g_zones.tiny_space) % TINY_SIZE == 0) {
			if (g_zones.tiny[(((unsigned long) (ptr) % (unsigned long) g_zones.tiny_space) / TINY_SIZE)].free == TRUE)
				free_error(ptr);
			g_zones.tiny[(((unsigned long) (ptr) % (unsigned long) g_zones.tiny_space) / TINY_SIZE)].free = TRUE;
			return;
		} else
			free_error(ptr);
	}
	if (ptr >= g_zones.small_space && ptr <= g_zones.small_space + (SMALL_SIZE * SMALL_BLOCKS)) {
		if (((unsigned long) (ptr) % (unsigned long) g_zones.small_space) % SMALL_SIZE == 0) {
			if (g_zones.small[(((unsigned long) (ptr) % (unsigned long) g_zones.small_space) / SMALL_SIZE)].free == TRUE)
				free_error(ptr);
			g_zones.small[(((unsigned long) (ptr) % (unsigned long) g_zones.small_space) / SMALL_SIZE)].free = TRUE;
			return;
		} else
			free_error(ptr);
	}
	return free_large(ptr);
}