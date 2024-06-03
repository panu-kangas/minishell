/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_com.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:38:10 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 11:12:58 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_concatenate_arrays(char **arr1, int size1, char **arr2, int size2)
{
	char	**result;
	int		i;
	int		j;

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
			return (NULL);
		j++;
	}
	result[size1 + j] = NULL;
	return (result);
}

int	add_to_arg(t_token *current, char **arr)
{
	int		size1;
	int		size2;
	char	**temp;

	size1 = ft_array_len(arr);
	size2 = ft_array_len(current->args);
	temp = ft_concatenate_arrays(arr, size1, current->args, size2);
	if (!temp)
		return (1);
	ft_free_doubleptr(current->args);
	current->args = temp;
	return (0);
}

static char	*expand_substr_com(char *str, t_env_lst *env_lst, t_token *current)
{
	char	*temp;
	char	**split;

	if (!str || !env_lst)
		return (NULL);
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
	free(str);
	return (temp);
}

char	*expand_str_com(char *str, t_env_lst *env_lst, t_token *current)
{
	char	*new;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i] != 0 && str[i] != '$')
		i++;
	ft_strlcpy(new, str, i - j + 1);
	j = i + 1;
	i++;
	while ((ft_isalnum(str[i]) == 1 || str[i] == '_') && str[i] != 0)
		i++;
	temp = ft_strjoin_free(ft_strdup(new), \
	expand_substr_com(ft_substr(str, j, i - j), env_lst, current));
	free (new);
	if (!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin_free(temp, ft_substr(str + j, 0, i));
	return (temp);
}

int	ft_iter_and_exp_com(t_parse *lst, t_env_lst *e_lst, t_token *cur)
{
	char	*temp;
	t_parse	*head;

	head = lst;
	while (head)
	{
		while (head->isexpand == 1 && ft_char_counter(head->str, '$') > 0)
		{
			if (*(ft_strchr(head->str, '$') + 1) == ' ' \
			|| *(ft_strchr(head->str, '$') + 1) == 0)
				break ;
			if (head->istrim != 0)
				temp = trim_str(expand_str_com(head->str, e_lst, cur));
			else
				temp = expand_str_com(head->str, e_lst, cur);
			if (!temp)
				return (write_sys_error("malloc error"));
			free(head->str);
			head->str = temp;
		}
		head = head->next;
	}
	return (0);
}
