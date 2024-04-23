/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:49:04 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 13:55:38 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1c;
	unsigned char	*s2c;
	int				equal;
	int				i;

	s1c = (unsigned char *)s1;
	s2c = (unsigned char *)s2;
	i = 0;
	equal = 0;
	while (n > 0)
	{
		if (s1c[i] != s2c[i])
		{
			equal = 1;
			break ;
		}
		i++;
		n--;
	}
	if (equal == 1)
		return (s1c[i] - s2c[i]);
	return (0);
}
