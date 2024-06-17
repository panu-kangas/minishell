/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:39:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/17 11:57:58 by pkangas          ###   ########.fr       */
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

int	all_non_found(t_env *env_lst, char *str, int *e_st)
{
	int	i;
	int	j;
	char	*var_name;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) == 1 || str[i + 1] == '_'))
		{
			j = ++i;
			while ((ft_isalnum(str[i]) == 1 || str[i] == '_') && str[i] != 0)
				i++;
			var_name = ft_substr(str, j, i - j);
			if (var_name == NULL)
			{
				*e_st = 1;
				return (write_sys_error("malloc failed"));
			}
			if (expand_env_var(env_lst, var_name) != NULL)
			{
				free(var_name);
				return (0);
			}
			free(var_name);
		}
		else if (str[i] != '$')
			return (0);
	}
	return (1);
}

static int	expand_filename(t_parse *t_p, t_env *env_lst, int e_st, size_t i)
{
	char	*temp;

	while (i < ft_strlen(t_p->str) && all_non_found(env_lst, t_p->str, &e_st) == 0)
	{
		if (t_p->str[i] == '$' && \
		((ft_isalnum(t_p->str[i + 1]) == 1 || t_p->str[i + 1] == '_')))
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
			i = 0;
		}
		else
			i++;
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
		if (temp_parse->isexpand == 1 && ft_char_counter(head->str, '$') > 0)
		{
			e_st = expand_filename(temp_parse, e_lst, e_st, 0);
			if (e_st != 0)
				return (e_st);
		}
		temp_parse = temp_parse->next;
	}
	return (e_st);
}
