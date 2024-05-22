/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/22 09:59:41 by tsaari           ###   ########.fr       */
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
	if (!new)
		return (NULL);
	while (lst)
	{
		new = ft_strjoin(new, lst->str);
		if (!new)
			return (NULL);
		free(lst->str);
		lst = lst->next;
	}
	return (new);
}

int ft_lstiter_and_expand(t_parse *lst, t_env_lst *env_lst, t_data *data)
{
	char *temp;
	t_parse *head;
	int exit_status;

	exit_status =  0;
	head = lst;
	exit_status = expand_prev_exit_code(head, data);
	if (exit_status != 0)
		return (exit_status);
	while (head)
	{
		while (head->isexpand == 1 && ft_char_counter(head->str, '$') > 0)
		{
			if (*(ft_strchr(head->str, '$') + 1) == ' ' || *(ft_strchr(head->str, '$') + 1) == 0)
				break ;
			temp = expand_str(head->str, env_lst);
			if (!temp)
				return (write_sys_error("malloc error"));
			free(head->str);
			head->str = temp;
		}
		head = head->next;
	}
	return (0);
}
