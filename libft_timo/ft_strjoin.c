/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:02:20 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/08 09:19:45 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	char	*s1c;
	char	*s2c;
	size_t	len;

	if (!s1 || ! s2)
		return (0);
	s2c = (char *)s2;
	s1c = (char *)s1;
	len = (ft_strlen(s1c) + ft_strlen(s2c)) * sizeof(char) + 1;
	res = (char *)ft_calloc(len, sizeof(char));
	if (res == 0)
		return (0);
	ft_memmove(res, s1c, ft_strlen(s1c));
	ft_strlcat(res, s2c, len);
	return (res);
}
