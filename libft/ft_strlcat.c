/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:46 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 15:01:48 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*srcc;
	size_t	len;
	size_t	i;

	srcc = (char *)src;
	i = 0;
	if ((!dst || !src) && !size)
		return (0);
	while (dst[i] != 0 && i < size)
		i++;
	if (i < size)
		len = i + ft_strlen(srcc);
	else
		return (size + ft_strlen(srcc));
	while (*srcc != 0 && (i + 1) < size)
	{
		dst[i] = *srcc++;
		i++;
	}
	dst[i] = '\0';
	return (len);
}
