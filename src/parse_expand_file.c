/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:39:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/29 14:35:52 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

int	check_file_name(char **temp, char *orig)
{
	char	**split;
	int		len;
	int		exit_status;

	len = 0;
	exit_status = 0;
	split = ft_split(*temp, ' ');
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



int ft_lstiter_and_expand_files(t_parse *lst, t_env_lst *env_lst, t_data *data, int exit_status)
{
	char *temp;
	int i;
	t_parse *head;

	head = lst;
	exit_status = expand_prev_exit_code(head, data);
	if (exit_status != 0)
		return (exit_status);
	while (head)
	{
		if (head->isexpand == 1)
		{
			i = 0;
			while (head->str[i] != 0)
			{
				if (head->str[i] == '$')
				{
					if (head->str[i + 1] == ' ' || head->str[i + 1] == 0)
						break ;
					if (head->istrim != 0)
						temp = trim_str(expand_str_file(head->str, env_lst));
					else
						temp = expand_str_file(head->str, env_lst);
					if (!temp)
						return (write_sys_error("malloc error"));
					exit_status = check_file_name(&temp, head->str);
					if (exit_status != 0)
						return (write_sys_error("malloc error"));
					free(head->str);
					head->str = temp;
				}
				i++;
			}
		}
		head = head->next;
	}
	return (exit_status);
}
