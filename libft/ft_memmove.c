/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 10:49:17 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:05:48 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned int			i;
	unsigned char			*char_dst;
	const unsigned char		*char_src;

	i = 0;
	char_dst = (unsigned char *)dst;
	char_src = (unsigned char *)src;
	if (char_src < char_dst)
	{
		i = len;
		while (i > 0)
		{
			i--;
			char_dst[i] = char_src[i];
		}
	}
	else
	{
		while (i < len && char_src != 0)
		{
			char_dst[i] = char_src[i];
			i++;
		}
	}
	return (dst);
}
