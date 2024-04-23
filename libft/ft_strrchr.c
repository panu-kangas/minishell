/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:44:19 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 14:34:41 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*sc;
	char	cc;
	int		len;

	cc = c;
	sc = (char *)s;
	len = ft_strlen(sc);
	while (len >= 0)
	{
		if (sc[len] != cc)
			len--;
		else
			return (sc + len);
	}
	return (0);
}
