/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion_tokens.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:22:11 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/31 10:23:07 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	expand_com(t_token *current, t_env_lst *env_lst, t_data *data, int exit_status)
{
	t_parse	*head;
	char	*temp;

	head = NULL;
	exit_status = handle_substrings(current->com, &head);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	exit_status = ft_lstiter_and_expand_com(head, env_lst, data, current);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	exit_status = ft_lst_iter_remove_quotes(head);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	temp = ft_lstiter_and_make_new_str(head);
	if (!temp)
	{
		ft_free_parse(head, 0);
		return (write_sys_error("malloc error"));
	}
	free(current->com);
	current->com = temp;
	ft_free_parse(head, 0);
	return (exit_status);
}

static int handle_arg_expansion(t_parse *head, t_env_lst *env_lst, t_data *data, int exit_status)
{
	exit_status = ft_lstiter_and_expand_arg(head, env_lst, data, exit_status);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	exit_status = ft_lst_iter_remove_quotes(head);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	return(exit_status);
}

static int	expand_args(t_token *current, t_env_lst *env_lst, t_data *data, int exit_status)
{
	t_parse	*head;
	char	*temp;
	int		i;

	i = -1;
	while (current->args[++i] != 0)
	{
		head = NULL;
		exit_status = handle_substrings(current->args[i], &head);
		if (exit_status != 0)
			return (ft_free_parse(head, exit_status));
		exit_status = handle_arg_expansion(head, env_lst, data, exit_status);
		if (exit_status != 0)
			return (exit_status);
		temp = ft_lstiter_and_make_new_str(head);
		if (!temp)
			return(ft_free_parse(head, 1));
		free(current->args[i]);
		current->args[i] = temp;
		ft_free_parse(head, 0);
	}
	return (0);
}


static int	expand_file(t_parse *head, t_env_lst *env_lst, t_data *data, char **temp)
{
	int	exit_status;

	exit_status = 0;
	exit_status = ft_lstiter_and_expand_files(head, env_lst, data, exit_status);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	exit_status = ft_lst_iter_remove_quotes(head);
	if (exit_status != 0)
		return (ft_free_parse(head, exit_status));
	*temp = ft_lstiter_and_make_new_str(head);
	if (!temp)
	{
		ft_free_parse(head, 0);
		return(write_sys_error("malloc error"));
	}
	return (0);
}

static int	expand_files(t_token *current, t_env_lst *env_lst, t_data *data, int exit_status)
{
	t_file	*tempfile;
	t_parse	*head;
	char	*temp;

	tempfile = current->files;
	while(tempfile != NULL)
	{
		head = NULL;
		if (tempfile->is_append == 1 && tempfile->is_infile == 1 \
		&& (tempfile->filename[0] == '"' || tempfile->filename[0] == '\''))
			tempfile->quoted_heredoc = 1;
		exit_status = handle_substrings(tempfile->filename, &head);
		if (exit_status != 0)
			return (ft_free_parse(head, exit_status));
		if (tempfile->is_append == 1 && tempfile->is_infile == 1)
			change_expand_status(head);
		exit_status = expand_file(head, env_lst, data, &temp);
		if (exit_status != 0)
			return (ft_free_parse(head, exit_status));
		free(tempfile->filename);
		tempfile->filename = temp;
		ft_free_parse(head, 0);
		tempfile = tempfile->next;
	}
	return (0);
}