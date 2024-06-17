/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/17 12:50:16 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_lstiter_and_make_new_str(t_parse *head)
{
	char	*new;
	char	*tempstr;
	t_parse	*temp;

	temp = head;
	new = ft_strdup("");
	if (!new)
		return (NULL);
	while (temp)
	{
		tempstr = new;
		new = ft_strjoin(new, temp->str);
		free (tempstr);
		if (!new)
			return (NULL);
		temp = temp->next;
	}
	return (new);
}

char	*trimmed_str(char *str, char *temp, int in_word, int i)
{
	char	*ret;
	int		j;

	j = 0;
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
	ret = ft_strdup(temp);
	free(temp);
	return (ret);
}

char	*trim_str(char *str)
{
	char	*temp;
	int		i;
	int		in_word;

	if (!str)
		return (NULL);
	i = 0;
	in_word = 0;
	while (str[i] == ' ')
		i++;
	temp = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (temp == NULL)
		return (NULL);
	temp = trimmed_str(str, temp, in_word, i);
	free(str);
	if (!temp)
		return (NULL);
	return (temp);
}

void	change_amb_status(t_parse *head, t_file *file)
{
	t_parse	*temp;

	temp = head;
	while (temp != NULL)
	{
		if ((temp->str[0] != '"' && temp->str[0] != '\'') \
		&& ft_char_counter(temp->str, '$') > 0)
			file->is_amb = 1;
		temp = temp->next;
	}
}

int	ft_lst_iter_remove_quotes(t_parse *head)
{
	char	*stret;
	t_parse	*temp;

	temp = head;
	while (temp)
	{
		if (temp->str[0] == '\'')
		{
			stret = ft_strtrim(temp->str, "'");
			if (!stret)
				return (write_sys_error("malloc error"));
			free(temp->str);
			temp->str = stret;
		}
		else if (temp->str[0] == '"')
		{
			stret = ft_strtrim(temp->str, "\"");
			if (!stret)
				return (write_sys_error("malloc error"));
			free(temp->str);
			temp->str = stret;
		}
		temp = temp->next;
	}
	return (0);
}
