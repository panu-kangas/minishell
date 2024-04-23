/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:47:05 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 13:52:17 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*sc;
	unsigned char	cc;
	int				i;

	sc = (unsigned char *)s;
	cc = (unsigned char)c;
	i = 0;
	while (n > 0)
	{
		if (sc[i] == cc)
			return (sc + i);
		i++;
		n--;
	}
	return (0);
}
