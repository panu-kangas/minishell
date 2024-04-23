/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:06:47 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/08 09:28:30 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	char	*sc;

	sc = (char *)s;
	if (!s)
		return (0);
	if (len + (size_t)start >= ft_strlen(sc))
		len = ft_strlen(sc) - start;
	if ((size_t)start >= ft_strlen(sc))
		return (ft_strdup(""));
	ret = (char *)malloc ((len + 1) * sizeof(char));
	if (!ret)
		return (0);
	sc += start;
	len += 1;
	ft_strlcpy(ret, sc, len);
	return (ret);
}
