/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:40 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/17 10:22:02 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *expand_exit_str(char *str, t_data *data, int i)
{
	char *temp;
	char *new;

	new = ft_substr(str, 0, i);
	if (!new)
		return (NULL);
	temp = new;
	new = ft_strjoin(new, (ft_itoa(data->prev_exit_status)));
	if (!new)
		return(NULL);
	free (temp);
	temp = new;
	i += 2;
	new = ft_strjoin(new, str + i);
	free (temp);
	if (!str)
		return (NULL);
	return (new);
}

int expand_prev_exit_code(t_parse *lst, t_data *data)
{
	char *temp;
	int i;

	i = 0;
	while (lst)
	{
		while (lst->str[i] != 0)
		{
			if (lst->str[i] == '$' && lst->str[i + 1] == '?')
			{
				temp = lst->str;
				lst->str = expand_exit_str(lst->str, data, i);
				if (!lst->str)
					return (-1);
				free (temp);
				i = 0;
				continue ;
			}
			i++;
		}
		lst = lst->next;
	}
	return (0);
}

int ft_lst_iter_remove_quotes(t_parse *lst)
{
	char *stret;
	while (lst)
	{
		if (lst->str[0] == '\'')
		{
			stret = ft_strtrim(lst->str, "'");
			if (!stret)
				return (-1);
			free(lst->str);
			lst->str = stret;
		}
		else if (lst->str[0] == '"')
		{
			stret = ft_strtrim(lst->str, "\"");
			if (!stret)
				return (-1);
			free(lst->str);
			lst->str = stret;
		}
		lst = lst->next;
	}
	return (0);
}


static int expand_com(t_token *current, t_env_lst *env_lst, t_data *data)
{
	t_parse *head;
	char *temp;

	head = NULL;
	if (handle_substrings(current->com, &head) != 0)
		return (-1);
	if (expand_prev_exit_code(head, data) != 0)
		return (-1);
	if (ft_lstiter_and_expand(head, env_lst) != 0)
		return (-1);
	if (ft_lst_iter_remove_quotes(head) != 0)
		return (-1);
	temp = ft_lstiter_and_make_new_str(head);
	if (!temp)
		return (-1);
	free(current->com);
	current->com = temp;
	free(head);
	return (0);
}

static int expand_args(t_token *current, t_env_lst *env_lst, t_data *data)
{
	t_parse *head;
	char *temp;
	int i;

	i = 0;
	while (current->args != NULL && current->args[i] != 0)
	{
		head = NULL;
		if (handle_substrings(current->args[i], &head) != 0)
			return (-1);
		if (expand_prev_exit_code(head, data) != 0)
			return (-1);
		if (ft_lstiter_and_expand(head, env_lst) != 0)
			return (-1);
		if (ft_lst_iter_remove_quotes(head) != 0)
			return (-1);
		temp = ft_lstiter_and_make_new_str(head);
		if (!temp)
			return (-1);
		free(current->args[i]);
		current->args[i] = temp;
		i++;
	}
	return (0);
}

static int expand_files(t_token *current, t_env_lst *env_lst, t_data *data)
{
	t_file	*tempfile;
	t_parse *head;
	char *temp;

	tempfile = current->files;
	while(tempfile != NULL)
	{
		head = NULL;
		if (handle_substrings(tempfile->filename, &head) != 0)
			return (-1);
		if (expand_prev_exit_code(head, data) != 0)
			return (-1);
		if (ft_lstiter_and_expand(head, env_lst) != 0)
			return (-1);
		if (ft_lst_iter_remove_quotes(head) != 0)
			return (-1);
		temp = ft_lstiter_and_make_new_str(head);
		if (!temp)
			return(-1);
		free(tempfile->filename);
		tempfile->filename = temp;
		tempfile = tempfile->next;
	}
	return (0);
}


static int expand_token(t_token *current, t_env_lst *env_lst, t_data *data)
{
	if(expand_com(current, env_lst, data) != 0)
		return (-1);
	if(expand_args(current, env_lst, data) != 0)
		return (-1);
	if(expand_files(current, env_lst, data) != 0)
		return (-1);
	return (0);
}

int parse_expansions(t_data *data, t_env_lst *env_lst)
{
	t_token	*temp_token;
	(void)env_lst;

	int exit_status;
	exit_status = 0;
	temp_token = data->tokens;
	while (temp_token != NULL)
	{
		exit_status = expand_token(temp_token, env_lst, data);
		if (exit_status != 0)
			return (write_sys_error("malloc error"));
		temp_token = temp_token->next;
	}
	return (exit_status);
}
