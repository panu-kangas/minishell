/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:18:36 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/09 11:00:38 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	i;
	unsigned int	j;

	if (dstsize == 0)
		return (ft_strlen(src));
	i = ft_strlen(dst);
	j = 0;
	while (src[j] != '\0' && i < (dstsize - 1))
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	if (dstsize > (i - j))
		dst[i] = '\0';
	if (dstsize < (i - j))
		return (dstsize + ft_strlen(src));
	return (ft_strlen(src) + (i - j));
}
