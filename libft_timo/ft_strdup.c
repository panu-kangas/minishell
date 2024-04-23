/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:12:53 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/16 10:55:00 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*scopy;

	len = 0;
	while (s1[len] != '\0')
		len++;
	scopy = (char *)malloc(len * sizeof(char) + 1);
	if (!scopy)
		return (0);
	ft_strlcpy (scopy, s1, len + 1);
	return (scopy);
}
