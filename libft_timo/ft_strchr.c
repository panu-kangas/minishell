/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:49:12 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/13 14:25:16 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*sc;
	int		i;
	int		len;

	i = 0;
	sc = (char *)s;
	len = ft_strlen(sc);
	while (len >= 0)
	{
		if (sc[i] != (char)c)
		{
			i++;
			len--;
		}
		else
			return (sc + i);
	}
	return (NULL);
}
