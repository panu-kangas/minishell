/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 09:07:51 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/17 09:23:24 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1c;
	unsigned char	*s2c;
	size_t			i;

	if (!s1 || !s2)
		return (0);
	if (n == 0)
		return (0);
	s1c = (unsigned char *)s1;
	s2c = (unsigned char *)s2;
	i = 0;
	while (!(*s1c == '\0' && *s2c == '\0') && i < n)
	{
		if (*s1c > *s2c)
			return (*s1c - *s2c);
		else if (*s1c < *s2c)
			return (*s1c - *s2c);
		i++;
		s1c++;
		s2c++;
	}
	return (0);
}
