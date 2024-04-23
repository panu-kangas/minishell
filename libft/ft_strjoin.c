/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:51:31 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:08:42 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*final_str;

	if (s1 == 0 || s2 == 0)
		return (0);
	i = 0;
	j = 0;
	final_str = (char *) malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (final_str == 0)
		return (0);
	while (s1[i] != '\0')
	{
		final_str[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
		final_str[j++] = s2[i++];
	final_str[j] = '\0';
	return (final_str);
}
