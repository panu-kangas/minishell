/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:20:35 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/08 09:22:36 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*sc;
	char			*res;
	unsigned int	i;

	if (!s)
		return (0);
	sc = (char *)s;
	i = 0;
	res = (char *)malloc(ft_strlen(sc) * sizeof(char) + 1);
	if (!res)
		return (0);
	while (sc[i] != '\0')
	{
		res[i] = f(i, sc[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
