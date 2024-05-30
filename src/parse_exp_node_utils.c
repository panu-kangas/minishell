/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 12:17:09 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_lstiter_and_make_new_str(t_parse *lst)
{
	char *new;
	char *temp;

	new = ft_strdup("");
	if (!new)
		return (NULL);
	while (lst)
	{
		temp = new;
		new = ft_strjoin(new, lst->str);
		free (temp);
		if (!new)
			return (NULL);
		lst = lst->next;
	}
	return (new);
}

char *trimmed_str(char *str, int in_word, int j, int i)
{
	char temp[ft_strlen(str)];

	while (str[i] != '\0')
	{
		if (str[i] != ' ')
		{
			temp[j++] = str[i++];
			in_word = 1;
		}
		else
		{
			while (str[i] == ' ')
				i++;
			if (in_word && str[i] != '\0')
				temp[j++] = ' ';
			in_word = 0;
		}
	}
	temp[j] = 0;
	return(ft_strdup(temp));
}

char *trim_str(char *str)
{
	char *temp;
	int i;
	int j;
	int in_word;

	if (!str)
		return (NULL);
 	i = 0;
	j = 0;
	in_word = 0;
	while (str[i] == ' ')
		i++;
	temp = trimmed_str(str, in_word, j, i);
	free(str);
	if (!temp)
		return (NULL);
	return(temp);
}

void change_expand_status(t_parse *head)
{
	t_parse *temp;

	temp = head;

	while(temp != NULL)
	{
		temp->isexpand = 0;
		temp = temp->next;
	}
}

int ft_lst_iter_remove_quotes(t_parse *lst)
{
	char *stret;
	while (lst)
	{
		if (lst->str[0] == '\'')
		{
			stret = ft_strtrim(lst->str, "'");
			if (!stret)
				return (-1);
			free(lst->str);
			lst->str = stret;
		}
		else if (lst->str[0] == '"')
		{
			stret = ft_strtrim(lst->str, "\"");
			if (!stret)
				return (-1);
			free(lst->str);
			lst->str = stret;
		}
		lst = lst->next;
	}
	return (0);
}