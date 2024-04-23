/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:14:53 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/09 11:21:08 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void	free_strings(char **result, int k)
{
	while (k >= 0)
	{
		free(result[k]);
		k--;
	}
	free(result);
}

static int	get_str_len(char const *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i] != c && s[i] != '\0')
	{
		len++;
		i++;
	}
	return (len);
}

static int	process_string(char const *s, char c, char **result, int k)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			len = get_str_len(s, c, i);
			result[k] = ft_substr(s, i, len);
			if (result[k] == 0)
			{
				free_strings(result, k);
				return (-1);
			}
			i = i + len;
			k++;
		}
		else
			i++;
	}
	return (k);
}

static int	get_str_count(char const *s, char c)
{
	unsigned int	i;
	unsigned int	count;
	unsigned int	c_count;

	i = 0;
	c_count = 0;
	count = 1;
	while (s[i] == c)
	{
		i++;
		c_count++;
	}
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			if (s[i + 1] != c && s[i + 1] != '\0')
				count++;
		}
		i++;
	}
	if (s[0] == 0 || c_count == ft_strlen(s))
		count = count - 1;
	return (count);
}

char	**ft_split(char const *s, char c)
{
	int		k;
	int		count;
	char	**result;

	if (s == NULL)
		return (NULL);
	count = get_str_count(s, c);
	result = (char **) malloc((count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	k = 0;
	k = process_string(s, c, result, k);
	if (k == -1)
		return (NULL);
	else
		result[k] = NULL;
	return (result);
}
