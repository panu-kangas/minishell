/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:39:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 08:33:10 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

static int	check_file_name(char **temp, char *orig)
{
	char	**split;
	int		len;
	int		exit_status;

	len = 0;
	exit_status = 0;
	split = ft_split(*temp, ' ');
	if(!split)
		return (1);
	while(split[len] != NULL)
		len++;
	if (len > 1)
	{
		*temp = ft_strjoin("$", orig);
		if (!(*temp))
			exit_status = (1);
	}
	ft_free_doubleptr(split);
	return (exit_status);
}

static int	expand_filename(t_parse *temp_parse, t_env_lst *env_lst, int exit_status)
{
	char *temp;
	int i;

	i = 0;
	while (temp_parse->str[i] != 0)
	{
		if (temp_parse->str[i] == '$')
		{
			if (temp_parse->str[i + 1] == ' ' || temp_parse->str[i + 1] == 0)
				break ;
			if (temp_parse->istrim != 0)
				temp = trim_str(expand_str_file(temp_parse->str, env_lst));
			else
				temp = expand_str_file(temp_parse->str, env_lst);
			if (!temp)
				return (write_sys_error("malloc error"));
			exit_status = check_file_name(&temp, temp_parse->str);
			if (exit_status != 0)
				return (write_sys_error("malloc error"));
			free(temp_parse->str);
			temp_parse->str = temp;
		}
		i++;
	}
	return(exit_status);
}

int ft_lstiter_and_expand_files(t_parse *head, t_env_lst *env_lst, t_data *data, int exit_status)
{
	t_parse *temp_parse;

	temp_parse = head;
	exit_status = expand_prev_exit_code(head, data);
	if (exit_status != 0)
		return (exit_status);
	while (temp_parse)
	{
		if (temp_parse->isexpand == 1)
		{
			exit_status = expand_filename(temp_parse, env_lst, exit_status);
			if (exit_status != 0)
				return(exit_status);
		}
		temp_parse = temp_parse->next;
	}
	return (exit_status);
}
