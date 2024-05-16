/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:59:20 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/16 13:40:32 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int get_quoted_string(char *str, int i, char **substring, char quote)
{
    int j;

    j = i + 1;
    while (str[j] != quote && str[j] != '\0')
        j++;
    if (str[j] == quote)
    {
        *substring = (char *)malloc(j - i + 2);
        if (*substring == NULL)
            return (-1);
        ft_strlcpy(*substring, str + i, j - i + 2);
        return (j - i + 1);
    }
    else
    {
		write_sys_error("syntax error, invalid amount of quotes");
        exit (258);
    }
}

static int handle_quoted_substring(char *str, int i, t_parse **head)
{
	char *substring;
	int length;
	char quote;
	int expand;

	quote = str[i];
	if (quote == '\'')
		expand = 0;
	else
		expand = 1;
	length = get_quoted_string(str, i, &substring, quote);
	if (length > 0)
	{
		t_parse *new = new_node(substring, expand);
		if (!new)
		{
			free(substring);
			return (-1);
		}
		ft_lstadd_back_parse(head, new);
		return (length);
	}
	return (0);
}

static int handle_non_quoted_substring(char *str, int i, t_parse **head)
{
	int j;
	char *substring;

	j = i;
	while (str[j] != '\0' && str[j] != '\'' && str[j] != '"')
		j++;
	substring = ft_substr(str, i, j - i);
	if (!substring)
		return (-1);
	t_parse *new = new_node(substring, 1);
	if (!new)
	{
		free(substring);
		return (-1);
	}
	ft_lstadd_back_parse(head, new);
	return (j - i);
}

int handle_substrings(char *str, t_parse **head)
{
	int i;
	int exit_status;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			exit_status = handle_quoted_substring(str, i, head);
			if (exit_status == -1)
				return (exit_status);
			i += exit_status;
		}
		else
		{
			exit_status = handle_non_quoted_substring(str, i, head);
			if (exit_status == -1)
				return (exit_status);
			i += exit_status;
		}
	}
	return (0);
}
