/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:39:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/17 09:48:42 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_file_name(char **temp, char *orig)
{
	char	**split;
	int		len;
	int		exit_status;

	len = 0;
	exit_status = 0;
	split = ft_split(*temp, ' ');
	if (!split)
		return (1);
	while (split[len] != NULL)
		len++;
	if (len > 1)
	{
		free(*temp);
		*temp = ft_strdup(orig);
		if (!(*temp))
			exit_status = (1);
	}
	ft_free_doubleptr(split);
	return (exit_status);
}

static int	expand_filename(t_parse *t_p, t_env *env_lst, int e_st, int i)
{
	char	*temp;

	while (t_p->str[++i] != 0)
	{
		if (t_p->str[i] == '$' && \
		(t_p->str[i + 1] != ' ' && t_p->str[i + 1] != 0))
		{
			if (t_p->istrim != 0)
				temp = trim_str(expand_str_file(t_p->str, env_lst, t_p->str[0], 1));
			else
				temp = expand_str_file(t_p->str, env_lst, t_p->str[0], 1);
			if (!temp)
				return (write_sys_error("malloc error"));
			if (t_p->str[0] != '\'' && t_p->str[0] != '"')
				e_st = check_file_name(&temp, t_p->str);
			if (e_st != 0)
				return (write_sys_error("malloc error"));
			free(t_p->str);
			t_p->str = temp;
			if (t_p->str[i] == '\0')
				break ;
		}
	}
	return (e_st);
}

int	ft_iter_and_exp_files(t_parse *head, t_env *e_lst, t_data *d, int e_st)
{
	t_parse	*temp_parse;

	temp_parse = head;
	e_st = expand_prev_exit_code(head, d);
	if (e_st != 0)
		return (e_st);
	while (temp_parse)
	{
		if (temp_parse->isexpand == 1)
		{
			e_st = expand_filename(temp_parse, e_lst, e_st, -1);
			if (e_st != 0)
				return (e_st);
		}
		temp_parse = temp_parse->next;
	}
	return (e_st);
}
