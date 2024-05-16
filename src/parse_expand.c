/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:07:27 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/16 14:56:36 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_char_counter(char *str, char c)
{
	int i;
	int ctr;

	if (!str)
		return (0);
	ctr = 0;
	i = 0;
	while(str[i] != 0)
	{
		if (str[i] == c)
			ctr++;
		i++;
	}
	return (ctr);
}

static char *expand_substr(char *str, t_env_lst *env_lst)
{
	int i;
	char *temp;
	char *expandable;

	i = 0;
	while (ft_isalnum(str[i]) == 1)
		i++;
	expandable = ft_substr(str, 0, i);
	if (expandable == NULL)
		return (NULL);
	if (expand_env_var(env_lst, expandable) != NULL)
	{
		temp = expandable;
		expandable = ft_strdup(expand_env_var(env_lst, expandable));
		free(temp);
	}
	else
	{
		temp = expandable;
		expandable = ft_strdup("");
		free (temp);
	}
	if (expandable == NULL)
		return (NULL);
	return (expandable);
}

char *expand_str(char *str, t_env_lst *env_lst)
{
	char *new;
	char *temp;
	int i;
	int j;

	i = 0;
	j = 0;
	new = NULL;
	while (str[i] != 0 && str[i] != '$')
		i++;
	new = ft_substr(str, j, i - j);
	if (!new)
		return (NULL);
	j = i + 1;
	i++;
	while (ft_isalnum(str[i]) == 1 && str[i] != 0)
		i++;

	temp = ft_substr(str, j , i - j);
	new = ft_strjoin(new, expand_substr(temp, env_lst));
	if(!new)
		return (NULL);
	free(temp);
	while (str[i] != 0)
		i++;
	temp = ft_substr(str, i - j + 1, i);
	new = ft_strjoin(new, temp);
	free (temp);
	return (new);

}
