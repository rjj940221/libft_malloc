/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/13 14:08:49 by rojones           #+#    #+#             */
/*   Updated: 2017/06/14 11:37:11 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "libft_malloc.h"

t_zones g_zones;

void	free_error(void *ptr)
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
	ft_putstr(": pointer being freed was not allocated\n");
	destructer();
	pthread_kill(thread, 6);
}

void	free_tiny(void *ptr)
{
	unsigned long	idx;

	idx = ((unsigned long)(ptr) % (unsigned long)g_zones.tiny_space);
	if (idx % TINY_SIZE == 0)
	{
		if (g_zones.tiny[(idx / TINY_SIZE)].free == TRUE)
			free_error(ptr);
		g_zones.tiny[(idx / TINY_SIZE)].free = TRUE;
	}
	else
		free_error(ptr);
}

void	free_small(void *ptr)
{
	unsigned long	idx;

	idx = ((unsigned long)(ptr) % (unsigned long)g_zones.small_space);
	if (idx % SMALL_SIZE == 0)
	{
		if (g_zones.small[(idx / SMALL_SIZE)].free == TRUE)
			free_error(ptr);
		g_zones.small[(idx / SMALL_SIZE)].free = TRUE;
		return ;
	}
	else
		free_error(ptr);
}

void	free_large(void *ptr)
{
	t_zone_link	*old;

	old = find_zone(ptr);
	if (!old)
	{
		free_error(ptr);
		return ;
	}
	if (old->previous)
		old->previous->next = old->next;
	if (old->next)
		old->next->previous = old->previous;
	munmap(old->location, old->size);
}

void	free(void *ptr)
{
	if (g_zones.init == 0)
		return (free_error(ptr));
	if (ptr == NULL)
		return ;
	if (ptr >= g_zones.tiny_space && ptr <= g_zones.tiny_space +
			(TINY_SIZE * TINY_BLOCKS))
		return (free_tiny(ptr));
	if (ptr >= g_zones.small_space && ptr <= g_zones.small_space +
			(SMALL_SIZE * SMALL_BLOCKS))
		return (free_small(ptr));
	return (free_large(ptr));
}
