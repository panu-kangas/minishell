/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:41:18 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 12:24:59 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_iter_and_exp_arg(t_parse *head, t_env_lst *e_lst, int i)
{
	char	*tempstr;
	t_parse	*temp;

	temp = head;
	while (temp != NULL)
	{
		while (temp->str[++i] != 0)
		{
			if ((temp->str[i] == '$' && temp->isexpand == 1) && \
			(temp->str[i + 1] != ' ' || temp->str[i + 1] != 0))
			{
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
