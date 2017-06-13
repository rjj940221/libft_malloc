/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_suport.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/13 14:33:19 by rojones           #+#    #+#             */
/*   Updated: 2017/06/13 14:34:35 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft_malloc.h"

t_zones g_zones;

t_zone_link	*find_zone(void *adr)
{
	t_zone_link	*iter;

	iter = g_zones.large;
	while (iter != NULL) {
		if (iter->location == adr)
			return (iter);
	}
	return NULL;
}

size_t		print_zone(t_block *zone, int num_blocks){
	int		i;
	size_t	bytes;

	bytes = 0;
	i = -1;
	while (++i < num_blocks){
		if (zone[i].free == FALSE) {
			ft_printf("%p - %p : %d bytes\n", zone[i].location, (zone[i].location + zone[i].size), zone[i].size);
			bytes += zone[i].size;
		}
	}
	return bytes;
}

size_t		print_large_zone(){
	size_t		bytes;
	t_zone_link	*tmp;

	tmp = g_zones.large;
	bytes = 0;
	while (tmp){
		ft_printf("%p - %p : %d bytes\n", tmp->location, (tmp->location + tmp->size), tmp->size);
		bytes += tmp->size;
		tmp = tmp->next;
	}
	return  bytes;
}

void		show_alloc_mem(){
	size_t	total_bytes;

	if (g_zones.init == 0)
		init();
	total_bytes = 0;
	ft_printf("TINY : %p\n", g_zones.tiny_space);
	total_bytes += print_zone(g_zones.tiny, TINY_BLOCKS);
	ft_printf("SMALL : %p\n", g_zones.small_space);
	total_bytes += print_zone(g_zones.small, SMALL_BLOCKS);
	ft_printf("LARGE : %p\n", (g_zones.small_space + (SMALL_BLOCKS * SMALL_SIZE)));
	total_bytes += print_large_zone();
	ft_printf("Total : %lu bytes", total_bytes);

}
