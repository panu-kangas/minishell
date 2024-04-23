/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 07:37:12 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 14:32:49 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*hc;
	char	*nc;
	int		l;

	hc = (char *) haystack;
	nc = (char *) needle;
	l = (int) len;
	if (!haystack && !len)
		return (0);
	if (l < 0)
		l = ft_strlen(hc);
	if (ft_strlen(nc) == 0)
		return (hc);
	l -= ft_strlen(nc);
	while (*hc != 0 && l >= 0)
	{
		if (*hc == nc[0])
		{
			if (ft_strncmp(hc, nc, ft_strlen(nc)) == 0)
				return (hc);
		}
		l--;
		hc++;
	}
	return (0);
}
