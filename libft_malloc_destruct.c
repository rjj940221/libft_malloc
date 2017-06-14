/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_destruct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 09:48:33 by rojones           #+#    #+#             */
/*   Updated: 2017/06/14 11:42:38 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

void	destructer(void)
{
	if (g_zones.init == 1)
	{
		munmap(g_zones.tiny_space, (size_t)(TINY_BLOCKS * TINY_SIZE));
		munmap(g_zones.small_space, (size_t)(SMALL_BLOCKS * SMALL_SIZE));
		g_zones.init = 0;
	}
}
