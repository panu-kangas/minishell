/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:41:18 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/31 12:26:04 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_iter_and_exp_arg(t_parse *temp, t_env_lst *e_lst, int i)
{
	char	*tempstr;

	while (temp)
	{
		while (temp->isexpand == 1 && temp->str[i] != 0)
		{
			if (temp->str[++i] == '$')
			{
				if (*(ft_strchr(temp->str, '$') + 1) == ' '\
				|| *(ft_strchr(temp->str, '$') + 1) == 0)
					break ;
				if (temp->istrim != 0)
					tempstr = trim_str(expand_str(temp->str, e_lst));
				else
					tempstr = expand_str(temp->str, e_lst);
				if (!tempstr)
					return (write_sys_error("malloc error"));
				free(temp->str);
				temp->str = tempstr;
			}
		}
		temp = temp->next;
	}
	return (0);
}
