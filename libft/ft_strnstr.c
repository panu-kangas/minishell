/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:00:33 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/09 11:51:25 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*search(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	int				j;
	char			*found;

	i = 0;
	while (haystack[i] != '\0' && i < len)
	{
		j = 0;
		while (haystack[i] == needle[j] && needle[j] != '\0' && i < len)
		{
			i++;
			j++;
		}
		if (needle[j] == '\0')
		{
			found = (char *)&haystack[i - j];
			return (found);
		}
		else if (j != 0 && haystack[i] == needle[0])
			i = i - j;
		if (haystack[i] != '\0')
			i++;
	}
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char			*found;

	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	if (len == 0)
		return (0);
	found = search(haystack, needle, len);
	return (found);
}
