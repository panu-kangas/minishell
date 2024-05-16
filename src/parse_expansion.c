/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:40 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/16 14:45:32 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_lst_iter_remove_quotes(t_parse *lst)
{
	char *stret;
	while (lst)
	{
		if (lst->str[0] == '\'')
		{
			stret = ft_strtrim(lst->str, "'");
			if (!stret)
				return (1);
			free(lst->str);
			lst->str = stret;
		}
		else if (lst->str[0] == '"')
		{
			stret = ft_strtrim(lst->str, "\"");
			if (!stret)
				return (1);
			free(lst->str);
			lst->str = stret;
		}
		lst = lst->next;
	}
	return (0);
}


static int expand_com(t_token *current, t_env_lst *env_lst, int exit_status)
{
	t_parse *head;
	char *temp;

	head = NULL;
	exit_status = handle_substrings(current->com, &head);
	if (exit_status != 0)
		return (exit_status);
	ft_lstiter_and_expand(head, env_lst);
	ft_lst_iter_remove_quotes(head);
	temp = ft_lstiter_and_make_new_str(head);
	if (!temp)
		return (-1);
	free(current->com);
	current->com = temp;
	free(head);

	return (0);
}

static int expand_args(t_token *current, t_env_lst *env_lst, int exit_status)
{
	(void)env_lst;
	t_parse *head;
	char *temp;
	int i;

	i = 0;
	while (current->args[i] != 0)
	{
		head = NULL;
		exit_status = handle_substrings(current->args[i], &head);
		if (exit_status != 0)
			return (-1);
		ft_lstiter_and_expand(head, env_lst);
		ft_lst_iter_remove_quotes(head);
		temp = ft_lstiter_and_make_new_str(head);
		if (!temp)
			return (-1);
		free(current->args[i]);
		current->args[i] = temp;
		i++;
	}
	return (exit_status);

}

static int expand_files(t_token *current, t_env_lst *env_lst, int exit_status)
{
	(void)env_lst;
	t_file	*tempfile;
	t_parse *head;
	char *temp;

	tempfile = current->files;
	while(tempfile != NULL)
	{
		head = NULL;

		exit_status = handle_substrings(current->files->filename, &head);
		if (exit_status != 0)
			return (-1);
		ft_lstiter_and_expand(head, env_lst);
		ft_lst_iter_remove_quotes(head);
		temp = ft_lstiter_and_make_new_str(head);
		if (!temp)
			return(-1);
		free(tempfile->filename);
		tempfile->filename = temp;
		tempfile = tempfile->next;
	}
	return (0);
}


static int expand_token(t_token *current, t_env_lst *env_lst, int exit_status)
{
	if(expand_com(current, env_lst, exit_status) != 0)
		return (-1);
	if(expand_args(current, env_lst, exit_status) != 0)
		return (-1);
	if(expand_files(current, env_lst, exit_status) != 0)
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
		exit_status = expand_token(temp_token, env_lst, exit_status);
		if (exit_status != 0)
			return (write_sys_error("malloc error"));
		temp_token = temp_token->next;
	}
	return (exit_status);
}
