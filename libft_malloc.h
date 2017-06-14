/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/13 14:31:13 by rojones           #+#    #+#             */
/*   Updated: 2017/06/14 07:22:14 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H
# define TINY_SIZE getpagesize()
# define TINY_BLOCKS 100
# define SMALL_SIZE 4 * getpagesize()
# define SMALL_BLOCKS 100
# define PROT_RW PROT_READ | PROT_WRITE
# define MAP_FLAGS MAP_ANON | MAP_PRIVATE
# define FALSE 0
# define TRUE 1
# include <stdlib.h>
# include <sys/mman.h>
# include "libft/libft.h"

typedef struct	s_block{
	size_t	size;
	int		free;
	void	*location;
}				t_block;

typedef struct	s_zone_link
{
	size_t				size;
	void				*location;
	struct s_zone_link	*next;
	struct s_zone_link	*previous;
}				t_zone_link;

typedef struct	s_zones{
	int			init;
	t_block		tiny[TINY_BLOCKS];
	void		*tiny_space;
	t_block		small[SMALL_BLOCKS];
	void		*small_space;
	t_zone_link	*large;
}				t_zones;

extern t_zones	g_zones;

void			free(void *ptr);

void			*malloc(size_t size);

void			*realloc(void *ptr, size_t size);

void			show_alloc_mem();

t_zone_link		*find_zone(void *adr);

void			init();

#endif
