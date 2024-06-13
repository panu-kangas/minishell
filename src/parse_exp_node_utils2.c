/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/13 10:41:46 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_exit_str(char *str, t_data *data, int i)
{
	char	*temp;
	char	*new;
	char	*prev_exit_stat;

	new = ft_substr(str, 0, i);
	if (!new)
		return (NULL);
	temp = new;
	prev_exit_stat = ft_itoa(data->prev_exit_status);
	new = ft_strjoin(new, prev_exit_stat);
	free(temp);
	free(prev_exit_stat);
	if (!new)
		return (NULL);
	temp = new;
	i += 2;
	new = ft_strjoin(new, str + i);
	free(temp);
	if (!str)
		return (NULL);
	return (new);
}

int	expand_prev_exit_code(t_parse *lst, t_data *data)
{
	char	*temp;
	t_parse	*head;
	int		i;

	head = lst;
	while (head)
	{
		i = 0;
		while (head->str[i] != 0)
		{
			if (head->str[i] == '$' && head->str[i + 1] == '?')
			{
				temp = head->str;
				head->str = expand_exit_str(head->str, data, i);
				free (temp);
				if (!head->str)
					return (write_sys_error("malloc error"));
				i = 0;
				continue ;
			}
			i++;
		}
		head = head->next;
	}
	return (0);
}

void	ft_lstadd_back_parse(t_parse **lst, t_parse *new)
{
	t_parse	*head;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	head = *lst;
	while (head->next != NULL)
		head = head->next;
	head->next = new;
}

t_parse	*new_node(char *str, int isexpand, int istrim)
{
	t_parse	*new;

	new = (t_parse *)malloc(sizeof(t_parse));
	if (!new)
		return (NULL);
	init_parse(new);
	new->str = str;
	new->istrim = istrim;
	new->isexpand = isexpand;
	new->next = NULL;
	return (new);
}

void	change_expand_status(t_parse *head, t_file *file)
{
	t_parse	*temp;

	temp = head;
	if (file->filename[0] == '"' || file->filename[0] == '\'')
		file->quoted_heredoc = 1;
	while (temp != NULL)
	{
		temp->isexpand = 0;
		temp = temp->next;
	}
}
