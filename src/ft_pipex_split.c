/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:43:05 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 11:43:07 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_str_len(char const *s, char c, int i)
{
	int		orig_i;
	char	quot;

	orig_i = i;
	while (1)
	{
		while (s[i] != c && s[i] != '\"' && s[i] != '\'' && s[i] != '\0')
			i++;
		if (s[i] == c || s[i] == '\0')
			return (i - orig_i);
		quot = s[i++];
		while (s[i] != quot && s[i] != '\0')
			i++;
		if (s[i] == '\0')
			return (i - orig_i);
		else if (s[i + 1] == c || s[i + 1] == '\0')
			return (i - orig_i + 1);
		i++;
	}
	return (0);
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
			if (result[k] == NULL)
			{
				ft_free_doubleptr(result);
				return (-1);
			}
			i = i + len;
			k++;
		}
		else if (s[i] != '\0')
			i++;
	}
	return (k);
}

static int	skip_quotation(char const *s, int i)
{
	int	orig_i;

	orig_i = i;
	if (s[i] == '\"')
	{
		i++;
		while (s[i] != '\"' && s[i] != '\0')
			i++;
	}
	else if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\'' && s[i] != '\0')
			i++;
	}
	if (s[i] == '\0')
		return (orig_i);
	else
		return (i);
}

static int	get_str_count(char const *s, char c)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	if (s[0] == '\0')
		return (0);
	while (s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			count++;
		if ((s[i] == '\"' || s[i] == '\''))
			i = skip_quotation(s, i);
		i++;
	}
	if (s[i] == '\0' && s[i - 1] != c)
		count++;
	if (count == 0 && s[i - 1] == c)
		count++;
	return (count);
}

char	**ft_pipex_split(char const *s, char c)
{
	int		k;
	char	**result;

	if (s == NULL)
		return (NULL);
	result = (char **) malloc((get_str_count(s, c) + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	k = 0;
	k = process_string(s, c, result, k);
	if (k == -1)
		return (NULL);
	else if (k == 0 && s[0] == c)
	{
		result[0] = ft_strdup(s);
		if (result[0] == NULL)
		{
			free(result);
			return (NULL);
		}
		result[1] = NULL;
	}
	else
		result[k] = NULL;
	return (result);
}
