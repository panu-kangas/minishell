/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion_tokens.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:22:11 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/13 10:08:00 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	expand_com(t_token *cur, t_env *e_lst, t_data *d, int e_status)
{
	t_parse	*head;
	char	*temp;

	head = NULL;
	if (handle_substrings(cur->com, &head) != 0)
		return (ft_free_parse(head, e_status));
	e_status = expand_prev_exit_code(head, d);
	if (e_status != 0)
		return (e_status);
	e_status = ft_iter_and_exp_com(head, e_lst, cur);
	if (e_status != 0)
		return (ft_free_parse(head, e_status));
	e_status = ft_lst_iter_remove_quotes(head);
	if (e_status != 0)
		return (ft_free_parse(head, e_status));
	temp = ft_lstiter_and_make_new_str(head);
	if (!temp)
	{
		ft_free_parse(head, 0);
		return (write_sys_error("malloc error"));
	}
	free(cur->com);
	cur->com = temp;
	ft_free_parse(head, 0);
	return (e_status);
}

static int	handle_arg_exp(t_parse *head, t_env *e_lst, t_data *d, int arg_nbr)
{
	int	e_st;

	e_st = expand_prev_exit_code(head, d);
	if (e_st != 0)
		return (ft_free_parse(head, e_st));
	e_st = ft_iter_and_exp_arg(head, e_lst, arg_nbr);
	if (e_st != 0)
		return (ft_free_parse(head, e_st));
	e_st = ft_lst_iter_remove_quotes(head);
	if (e_st != 0)
		return (ft_free_parse(head, e_st));
	return (0);
}

int	expand_args(t_token *cur, t_env *e_lst, t_data *d, int e_st)
{
	t_parse	*head;
	int		i;
	char	*tempstr;

	i = 0;
	while (cur->args[i] != 0)
	{
		head = NULL;
		e_st = handle_substrings(cur->args[i], &head);
		if (e_st != 0)
			return (ft_free_parse(head, e_st));
		e_st = handle_arg_exp(head, e_lst, d, i);
		if (e_st != 0)
			return (e_st);
		tempstr = ft_lstiter_and_make_new_str(head);
		if (!tempstr)
			return (ft_free_parse(head, 1));
		free(cur->args[i]);
		cur->args[i] = tempstr;
		ft_free_parse(head, 0);
		i++;
	}
	return (0);
}

static int	expand_file(t_parse *head, t_env *e_lst, t_data *d, char **temp)
{
	int	exit_status;

	exit_status = 0;
	exit_status = ft_iter_and_exp_files(head, e_lst, d, exit_status);
	if (exit_status != 0)
		return (exit_status);
	exit_status = ft_lst_iter_remove_quotes(head);
	if (exit_status != 0)
		return (exit_status);
	*temp = ft_lstiter_and_make_new_str(head);
	if (!(*temp))
		return (write_sys_error("malloc error"));
	return (0);
}

int	expand_files(t_token *cur, t_env *e_lst, t_data *d, int e_st)
{
	t_file	*tempfile;
	t_parse	*head;
	char	*temp;

	tempfile = cur->files;
	while (tempfile != NULL)
	{
		head = NULL;
		e_st = handle_substrings(tempfile->filename, &head);
		if (e_st != 0)
			return (ft_free_parse(head, e_st));
		if (tempfile->is_append == 1 && tempfile->is_infile == 1)
			change_expand_status(head, tempfile);
		else
			change_amb_status(head, tempfile);
		e_st = expand_file(head, e_lst, d, &temp);
		if (e_st != 0)
			return (ft_free_parse(head, e_st));
		free(tempfile->filename);
		tempfile->filename = temp;
		ft_free_parse(head, 0);
		tempfile = tempfile->next;
	}
	return (0);
}
