/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_realloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/13 14:34:44 by rojones           #+#    #+#             */
/*   Updated: 2017/06/14 09:16:53 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "libft_malloc.h"

t_zones g_zones;

void	realloc_error(void *ptr)
{
	pid_t		id;
	pthread_t	thread;
	char		name[15];

	id = getpgrp();
	thread = pthread_self();
	pthread_getname_np(thread, name, 15);
	ft_putstr(name);
	ft_putchar('(');
	ft_putnbr(id);
	ft_putchar(',');
	print_address(thread);
	ft_putstr(") libft_malloc: *** error for object ");
	print_address(ptr);
	ft_putstr(": pointer being realloc'd was not allocated\n");
	munmap(g_zones.tiny_space, (size_t)(TINY_BLOCKS * TINY_SIZE));
	munmap(g_zones.small_space, (size_t)(SMALL_BLOCKS * SMALL_SIZE));
	pthread_kill(thread, 6);
}

void	*realloc_large(void *ptr, size_t size)
{
	t_zone_link	*old;
	void		*re;

	old = find_zone(ptr);
	if (!old)
	{
		realloc_error(ptr);
		return (NULL);
	}
	re = mmap(0, size, PROT_RW, MAP_FLAGS, -1, 0);
	if (re != MAP_FAILED)
	{
		memcpy(re, old->location, (size < old->size) ? size : old->size);
		if (munmap(ptr, old->size))
			return (NULL);
		old->location = re;
		old->size = size;
		return (re);
	}
	return (ptr);
}

void	*realloc_small(void *ptr, size_t size)
{
	void			*re;
	unsigned long	index;

	if (((unsigned long)(ptr) % (unsigned long)g_zones.small_space)
			% SMALL_SIZE != 0)
		realloc_error(ptr);
	index = ((unsigned long)(ptr) % (unsigned long)g_zones.small_space)
		/ SMALL_SIZE;
	if (size <= (size_t)SMALL_SIZE && size > (size_t)TINY_SIZE)
	{
		g_zones.small[index].size = size;
		return (g_zones.small[index].location);
	}
	re = malloc(size);
	if (re || size == 0)
	{
		g_zones.small[index].free = TRUE;
		memcpy(re, g_zones.small[index].location,
				(g_zones.small[index].size < size) ?
				g_zones.small[index].size : size);
		return (re);
	}
	return (g_zones.small[index].location);
}

void	*realloc_tiny(void *ptr, size_t size)
{
	void			*re;
	unsigned long	index;

	if (((unsigned long)(ptr) % (unsigned long)g_zones.tiny_space)
			% TINY_SIZE != 0)
		realloc_error(ptr);
	index = ((unsigned long)(ptr) % (unsigned long)g_zones.tiny_space)
		/ TINY_SIZE;
	if (size <= (size_t)TINY_SIZE)
	{
		g_zones.tiny[index].size = size;
		return (g_zones.tiny[index].location);
	}
	re = malloc(size);
	if (re || size == 0)
	{
		g_zones.tiny[index].free = TRUE;
		memcpy(re, g_zones.tiny[index].location,
				(g_zones.tiny[index].size < size)
				? g_zones.tiny[index].size : size);
		return (re);
	}
	return (g_zones.tiny[index].location);
}

void	*realloc(void *ptr, size_t size)
{
	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr >= g_zones.tiny_space &&
			ptr <= g_zones.tiny_space + (TINY_SIZE * TINY_BLOCKS))
		return (realloc_tiny(ptr, size));
	if (ptr >= g_zones.small_space &&
			ptr <= g_zones.small_space + (SMALL_SIZE * SMALL_BLOCKS))
		return (realloc_small(ptr, size));
	return (realloc_large(ptr, size));
}
