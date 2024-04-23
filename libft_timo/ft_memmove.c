/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:39:18 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 14:06:13 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dstc;
	unsigned const char	*srcc;
	unsigned long		i;

	i = 0;
	dstc = dst;
	srcc = src;
	if (dst == src || len == 0)
		return (dst);
	if (dstc < srcc)
	{
		while (i < len)
		{
			dstc[i] = srcc[i];
			i++;
		}
	}
	else
		while (len-- > 0)
			dstc[len] = srcc[len];
	return (dst);
}
