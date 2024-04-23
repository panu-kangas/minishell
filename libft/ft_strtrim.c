/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:54:46 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:08:58 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	get_trim_index_end(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = ft_strlen(s1) - 1;
	j = 0;
	while (i >= 0)
	{
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			break ;
		i--;
		j = 0;
	}
	return (i);
}

static int	get_trim_index_beg(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			break ;
		i++;
		j = 0;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int		trim_index_beg;
	unsigned int		trim_index_end;
	int					j;
	char				*final_str;

	if (s1 == 0)
		return (0);
	if (*s1 == '\0')
		return (ft_strdup(s1));
	trim_index_beg = get_trim_index_beg(s1, set);
	if (trim_index_beg == ft_strlen(s1))
	{
		final_str = (char *)malloc(1);
		*final_str = '\0';
		return (final_str);
	}
	trim_index_end = get_trim_index_end(s1, set);
	final_str = (char *) malloc((trim_index_end - trim_index_beg + 1) + 1);
	if (final_str == 0)
		return (0);
	j = 0;
	while (trim_index_beg <= trim_index_end)
		final_str[j++] = s1[trim_index_beg++];
	final_str[j] = '\0';
	return (final_str);
}
