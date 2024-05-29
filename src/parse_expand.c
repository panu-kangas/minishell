/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:07:27 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/29 10:11:08 by tsaari           ###   ########.fr       */
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

int ft_array_len(char **arr)
{
	int len;

	if (!arr)
		return (0);
	len = 0;
	while(arr[len] != NULL)
		len++;
	return(len);
}

char** ft_concatenate_arrays(char **arr1, int size1, char **arr2, int size2)
{
	char **result;
	int i;
	int j;

	i = 0;
	j = 0;
	result = malloc((size1 + size2 + 1) * sizeof(char *));
	if (result == NULL)
			return (NULL);
	while (i < size1)
	{
		result[i] = ft_strdup(arr1[i]);
		if (result[i] == NULL)
			return (NULL);
		i++;
	}
	while (j < size2)
	{
		result[size1 + j] = ft_strdup(arr2[j]);
		if (result[size1 + j] == NULL)
			return(NULL);
		j++;
	}
	result[size1 + j] = NULL;
	return (result);
}

int add_to_arg(t_token *current, char **arr)
{
	int size1;
	int size2;
	char **temp;

	size1 = ft_array_len(arr);
	size2= ft_array_len(current->args);
	temp = ft_concatenate_arrays(arr, size1, current->args, size2);
	if(!temp)
		return(1);
	ft_free_doubleptr(current->args);
	current->args = temp;
	return (0);
}

static char	*expand_substr_com(char *str, t_env_lst *env_lst, t_token *current)
{
	char	*temp;
	int		len;
	char	**split;

	len = 0;
	if (!str || !env_lst)
		return(NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
		temp = ft_strdup("");
	split = ft_split(temp, ' ');
	if (ft_array_len(split) > 1)
	{
		add_to_arg(current, split + 1);
		free(temp);
		temp = ft_strdup(split[0]);
	}
	ft_free_doubleptr(split);
	return (temp);
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

char *expand_str_com(char *str, t_env_lst *env_lst, t_token *current)
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
	temp = ft_strjoin(new, expand_substr_com(ft_substr(str, j , i - j), env_lst, current));
	if(!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin(temp, ft_substr(str + j, 0, i));
	return (temp);
}

