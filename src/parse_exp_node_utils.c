/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/20 15:47:07 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_lstadd_back_parse(t_parse **lst, t_parse *new)
{
	t_parse *temp;

	if (*lst == NULL)
	{
		*lst = new;
		return;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

t_parse *new_node(char *str,  int isexpand)
{
	t_parse *new;

	new = (t_parse *)malloc(sizeof(t_parse));
	if (!new)
		return NULL;
	init_parse(new);
	new->str = str;
	new->isexpand = isexpand;
	new->next = NULL;
	return new;
}

char *ft_lstiter_and_make_new_str(t_parse *lst)
{
	char *new;

	new = ft_strdup("");
	while (lst)
	{
		new = ft_strjoin(new, lst->str);
		free(lst->str);
		lst = lst->next;
	}
	return (new);
}

int ft_lstiter_and_expand(t_parse *lst, t_env_lst *env_lst)
{
	char *stret;
	while (lst)
	{
		if (lst->isexpand == 1 && ft_char_counter(lst->str, '$') > 0)
		{
			stret = expand_str(lst->str, env_lst);
			if (!stret)
				return (1);
			free(lst->str);
			lst->str = stret;
		}
		lst = lst->next;
	}
	return (0);
}