/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_suport.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/13 14:33:19 by rojones           #+#    #+#             */
/*   Updated: 2017/06/14 09:28:31 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

t_zones g_zones;

t_zone_link	*find_zone(void *adr)
{
	t_zone_link	*iter;

	iter = g_zones.large;
	while (iter != NULL)
	{
		if (iter->location == adr)
			return (iter);
	}
	return (NULL);
}

void		print_address(void *ptr)
{
	static char		*rep;
	static char		buffer[50];
	char			*iter;
	unsigned long	num;

	num = (unsigned long)ptr;
	iter = &buffer[49];
	rep = "0123456789abcdef";
	*iter = '\0';
	if (num == 0)
		*--iter = '0';
	while (num != 0)
	{
		*--iter = rep[num % 16];
		num /= 16;
	}
	*--iter = 'x';
	*--iter = '0';
	ft_putstr(iter);
}

size_t		print_zone(t_block *zone, int num_blocks)
{
	int		i;
	size_t	bytes;

	bytes = 0;
	i = -1;
	while (++i < num_blocks)
	{
		if (zone[i].free == FALSE)
		{
			print_address(zone[i].location);
			ft_putstr(" - ");
			print_address((zone[i].location + zone[i].size));
			ft_putnbr(zone[i].size);
			ft_putstr(" bytes\n");
			bytes += zone[i].size;
		}
	}
	return (bytes);
}

size_t		print_large_zone(void)
{
	size_t		bytes;
	t_zone_link	*tmp;

	tmp = g_zones.large;
	bytes = 0;
	while (tmp)
	{
		print_address(tmp->location);
		ft_putstr(" - ");
		print_address((tmp->location + tmp->size));
		ft_putnbr(tmp->size);
		ft_putstr(" bytes\n");
		bytes += tmp->size;
		tmp = tmp->next;
	}
	return (bytes);
}

void		show_alloc_mem(void)
{
	size_t	total_bytes;

	if (g_zones.init == 0)
		init();
	total_bytes = 0;
	ft_putstr("TINY : ");
	print_address(g_zones.tiny_space);
	ft_putchar('\n');
	total_bytes += print_zone(g_zones.tiny, TINY_BLOCKS);
	ft_putstr("SMALL : ");
	print_address(g_zones.small_space);
	ft_putchar('\n');
	total_bytes += print_zone(g_zones.small, SMALL_BLOCKS);
	ft_putstr("LARGE : ");
	print_address((g_zones.small_space + (SMALL_BLOCKS * SMALL_SIZE)));
	ft_putchar('\n');
	total_bytes += print_large_zone();
	ft_printf("Total : %lu bytes\n", total_bytes);
}
