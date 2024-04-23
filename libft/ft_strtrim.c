/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:04:36 by tsaari            #+#    #+#             */
/*   Updated: 2024/02/24 10:54:03 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*removebeginning(char const *s1, char *setc)
{
	int		i;
	char	*s1c;

	s1c = (char *)s1;
	i = 0;
	while (ft_memchr(setc, s1c[i], ft_strlen(setc)) != 0)
		i++;
	s1c = s1c + i;
	return (s1c);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s1c;
	size_t	e;
	char	*ret;

	if (!s1 || !set)
		return (0);
	s1c = removebeginning(s1, (char *)set);
	if (ft_strlen(s1c) != 0)
	{
		e = ft_strlen(s1c) - 1;
		while (ft_memchr((char *)set, s1c[e], ft_strlen((char *)set)) != 0)
			e--;
		ret = (char *)malloc((e * sizeof(char)) + 2);
		if (!ret)
			return (0);
		ft_strlcpy(ret, s1c, e + 2);
	}
	else
	{
		ret = ft_strdup("");
		if (!ret)
			return (0);
	}
	return (ret);
}
