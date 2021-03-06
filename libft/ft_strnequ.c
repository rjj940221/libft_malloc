/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 18:08:29 by rojones           #+#    #+#             */
/*   Updated: 2016/06/03 17:08:12 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strnequ(char const *s1, char const *s2, size_t n)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && i < (int)n && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
		if ((s1[i] == '\0' && s2[i] == '\0') || i == (int)n)
			return (1);
	}
	return (0);
}
