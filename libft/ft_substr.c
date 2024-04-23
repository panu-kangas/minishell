/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:44:43 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/09 12:41:47 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void	get_str(char const *s, char *substr, unsigned int start, size_t len)
{
	unsigned int	i;

	i = 0;
	while (s[start] != '\0' && i < len)
		substr[i++] = s[start++];
	substr[i] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	int				len_substr;
	char			*substr;

	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
	{
		substr = (char *) malloc(1);
		if (substr == 0)
			return (0);
		substr[0] = 0;
		return (substr);
	}
	if (len <= (ft_strlen(s) - start))
		len_substr = len;
	else
		len_substr = (ft_strlen(s) - start);
	substr = (char *) malloc(len_substr + 1);
	if (substr == 0)
		return (0);
	get_str(s, substr, start, len);
	return (substr);
}
