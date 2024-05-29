/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/29 11:43:45 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_exit_str(char *str, t_data *data, int i)
{
	char *temp;
	char *new;

	new = ft_substr(str, 0, i);
	if (!new)
		return (NULL);
	temp = new;
	new = ft_strjoin(new, (ft_itoa(data->prev_exit_status)));
	if (!new)
		return(NULL);
	free (temp);
	temp = new;
	i += 2;
	new = ft_strjoin(new, str + i);
	free (temp);
	if (!str)
		return (NULL);
	return (new);
}


int expand_prev_exit_code(t_parse *lst, t_data *data)
{
	char *temp;
	t_parse *head;
	int i;

	i = 0;
	head = lst;
	while (head)
	{
		while (head->str[i] != 0)
		{
			if (head->str[i] == '$' && head->str[i + 1] == '?')
			{
				temp = head->str;
				head->str = expand_exit_str(head->str, data, i);
				if (!head->str)
					return (-1);
				free (temp);
				i = 0;
				continue ;
			}
			i++;
		}
		head = head->next;
	}
	return (0);
}

void ft_lstadd_back_parse(t_parse **lst, t_parse *new)
{
	t_parse *head;

	if (*lst == NULL)
	{
		*lst = new;
		return;
	}
	head = *lst;
	while (head->next != NULL)
		head = head->next;
	head->next = new;
}

t_parse *new_node(char *str,  int isexpand, int istrim)
{
	t_parse *new;

	new = (t_parse *)malloc(sizeof(t_parse));
	if (!new)
		return NULL;
	init_parse(new);
	new->str = str;
	new->istrim = istrim;
	new->isexpand = isexpand;
	new->next = NULL;
	return new;
}


