/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand-args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:41:18 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/29 11:42:21 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"


int ft_lstiter_and_expand_arg(t_parse *lst, t_env_lst *env_lst, t_data *data, int exit_status)
{
	char *temp;
	int i;
	t_parse *head;

	i = 0;
	head = lst;
	exit_status = expand_prev_exit_code(head, data);
	if (exit_status != 0)
		return (exit_status);
	while (head)
	{
		while (head->isexpand == 1 && head->str[i] != 0)
		{
			if (head->str[i] == '$')
			{
				if (*(ft_strchr(head->str, '$') + 1) == ' ' || *(ft_strchr(head->str, '$') + 1) == 0)
				break ;
				if (head->istrim != 0)
					temp = trim_str(expand_str(head->str, env_lst));
				else
					temp = expand_str(head->str, env_lst);
				if (!temp)
					return (write_sys_error("malloc error"));
				free(head->str);
				head->str = temp;
			}
			i++;
		}
		head = head->next;
	}
	return (0);
}