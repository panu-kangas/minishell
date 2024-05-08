/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:40 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/08 12:43:14 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *check_env_list(char *str, t_env_lst *env_lst)
{
	if (ft_strncmp(env_lst->name, str, ft_strlen(str) + 1) == 0)
			return (env_lst->value);
	return (NULL);
}


int ft_char_counter(char *str, char c)
{
	int i;
	int ctr;

	ctr = 0;
	i = -1;
	while(str[++i] != 0)
		if (str[i] == c)
			ctr++;
	return (ctr);
}

char *make_str(char **arr, char *str, char *temp, int j)
{
	int k;
	int i;

	k = 0;
	while(k < j)
	{
		i = 0;
		while(str[i] != '$')
			i++;
		str[i] = 0;
		temp = ft_strjoin(temp, str);
		if(!temp)
			return(NULL);
		temp = ft_strjoin(temp, arr[k]);
		if(!temp)
			return(NULL);
		while(ft_isalnum(str[i]) == 1)
			i++;
		str += i;
		k++;
	}
	temp = ft_strjoin(temp, str);
	if (!temp)
		return (NULL);
	return (temp);
}

char *make_expanded_str(char **arr, char *str)
{
	int j;
	int k;
	char *temp;

	j = 0;
	k = 0;
	j = ft_char_counter(str, '$');
	temp = ft_strdup("");
	if(!temp)
			return(NULL);
	temp = make_str(arr, str, temp, j);
	if (!temp)
		return (NULL);
	return (temp);
}

char *expand(char *str, char **expandable, t_env_lst *env_lst)
{
	int i;
	char *temp;

	i = 1;
	while (ft_isalnum(str[i]) == 1)
		i++;
	*expandable = ft_substr(str, 1, i - 1);
	if (*expandable == NULL)
		return (NULL);
	if (expand_env_var(env_lst, *expandable) != NULL)
	{
		temp = *expandable;
		*expandable = ft_strdup (expand_env_var(env_lst, *expandable));
		free (temp);
	}
	else
	{
		temp = *expandable;
		*expandable = ft_strdup("");
		free (temp);
	}
	if (*expandable == NULL)
		return (NULL);
	return (*expandable);
}

char *expand_str(char *str, t_env_lst *env_lst)
{
	char *expandable;
	char *temp;
	int i;
	int j;
	char **arr;

	i = 0;
	arr = (char **)malloc(sizeof(char *) * i + 1);
	if (!arr)
		return (NULL);
	j = 0;
	expandable = NULL;
	while(str[i] != 0)
	{
		if (str[i] == '$')
		{
			expandable = expand(str + i, &expandable, env_lst);
			if (!expandable)
			{
				ft_free_double(arr);
				return(NULL);
			}
			arr[j] = expandable;
			j++;
		}
		i++;
	}
	arr[j] = 0;
	temp = make_expanded_str(arr, str);
	if (temp == NULL)
		return (NULL);
	return (temp);
}

int expand_args(t_token *current, int exit_status, t_env_lst *env_lst)
{
	int i;
	char *newstr;

	i = 0;
	while (current->args[i] != 0)
	{
		newstr = expand_str(current->args[i], env_lst);
		if (!newstr)
			return (write_sys_error("malloc failed")); // if malloc fails do I need to handle rest of args
		else
		{
				if (current->args[i])
					free(current->args[i]);
				current->args[i] = newstr;
		}
		i++;
	}
	return (exit_status);
}


int parse_expansions(t_data *data, t_env_lst *env_lst)
{
	t_token	*temp_token;
	char	*newstr;
	int exit_status;

	exit_status = 0;
	temp_token = data->tokens;
	while (temp_token != NULL)
	{
		if (ft_char_counter(temp_token->com, '$') > 0)
		{
			newstr = expand_str(temp_token->com, env_lst);
			if (!newstr)
				return (write_sys_error("malloc failed"));
			else
			{
				if (temp_token->com)
					free(temp_token->com);
				temp_token->com = newstr;
			}
		}
		exit_status = expand_args(temp_token, exit_status, env_lst); //what to do if there is a malloc fail, do we handle yhe rest
		temp_token = temp_token->next;
	}
	return (exit_status);
}
