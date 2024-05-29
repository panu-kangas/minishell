/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:07:27 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/29 14:23:02 by tsaari           ###   ########.fr       */
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

static char	*expand_substr(char *str, t_env_lst *env_lst)
{
	char	*temp;
	int		len;

	len = 0;
	if (!str || !env_lst)
		return(NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
		temp = ft_strdup("");
	return (temp);
}

static char	*expand_substr_file(char *str, t_env_lst *env_lst)
{
	char	*temp;
	int		len;

	len = 0;
	if (!str || !env_lst)
		return(NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
	{
		temp = ft_strdup(str);
	}
	return (temp);
}

char *expand_str(char *str, t_env_lst *env_lst)
{
	char new[ft_strlen(str) + 1];
	char *temp;
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] != 0 && str[i] != '$')
		i++;
	ft_strlcpy(new, str, i - j + 1);
	j = i + 1;
	i++;
	while (ft_isalnum(str[i]) == 1 && str[i] != 0)
		i++;
	temp = ft_strjoin(new, expand_substr(ft_substr(str, j , i - j), env_lst));
	if(!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin(temp, ft_substr(str + j, 0, i));
	return (temp);
}

char *expand_str_file(char *str, t_env_lst *env_lst)
{
	char new[ft_strlen(str) + 1];
	char *temp;
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] != 0 && str[i] != '$')
		i++;
	ft_strlcpy(new, str, i - j + 1);
	j = i + 1;
	i++;
	while (ft_isalnum(str[i]) == 1 && str[i] != 0)
		i++;
	temp = ft_strjoin(new, expand_substr_file(ft_substr(str, j , i - j), env_lst));
	if(!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin(temp, ft_substr(str + j, 0, i));
	return (temp);
}
