/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exp_node_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:07:44 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 14:36:21 by tsaari           ###   ########.fr       */
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

char	*trimmed_str(char *str, int in_word, int j, int i)
{
	char	*temp;
	char	*ret;

	temp = malloc(sizeof(char) * (ft_strlen(str) + 1));
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
	free (temp);
	return (ret);
}

char	*trim_str(char *str)
{
	char	*temp;
	int		i;
	int		j;
	int		in_word;

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
	return (temp);
}

void	change_amb_status(t_parse *head, t_file *file)
{
	t_parse	*temp;

	temp = head;
	if (file->filename[0] == '"' || file->filename[0] == '\'')
		file->quoted_heredoc = 1;
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
				return (-1);
			free(temp->str);
			temp->str = stret;
		}
		else if (temp->str[0] == '"')
		{
			stret = ft_strtrim(temp->str, "\"");
			if (!stret)
				return (-1);
			free(temp->str);
			temp->str = stret;
		}
		temp = temp->next;
	}
	return (0);
}
