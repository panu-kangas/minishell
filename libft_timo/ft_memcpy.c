/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:05:25 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/16 10:23:25 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dstc;
	unsigned const char	*srcc;
	unsigned int		i;

	dstc = dst;
	srcc = src;
	i = 0;
	if (!dst && !src && (dst == src))
		return (dst);
	while (i < n)
	{
		dstc[i] = srcc[i];
		i++;
	}
	return (dst);
}
