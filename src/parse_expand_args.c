/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:41:18 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/13 10:05:59 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_iter_and_exp_arg(t_parse *head, t_env *e_lst, int arg_nbr)
{
	char	*tempstr;
	t_parse	*temp;

	temp = head;
	while (temp)
	{
		while (temp->isexpand == 1 && ft_char_counter(temp->str, '$') > 0)
		{
			if (*(ft_strchr(temp->str, '$') + 1) == ' ' \
			|| *(ft_strchr(temp->str, '$') + 1) == 0)
				break ;
			if (temp->istrim != 0)
				tempstr = trim_str(expand_str(temp->str, e_lst, arg_nbr));
			else
				tempstr = expand_str(temp->str, e_lst, arg_nbr);
			if (!tempstr)
				return (write_sys_error("malloc error"));
			free(temp->str);
			temp->str = tempstr;
		}
		temp = temp->next;
	}
	return (0);
}
