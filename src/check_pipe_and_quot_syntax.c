/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe_and_quot_syntax.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:18:22 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/30 15:18:23 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_quot_pair(char *input, int i)
{
	char	c;

	c = input[i];
	i++;
	while (input[i] != c && input[i] != '\0')
		i++;
	if (input[i] == c)
		return (1);
	else
		return (0);
}

int	check_quot_syntax(t_data *data)
{
	char	*input;
	int		i;
	char	c;

	input = data->input;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (find_quot_pair(input, i) == 0)
				return (write_error(NULL, "syntax error", \
				"unclosed quotation"));
			c = input[i];
			i++;
			while (input[i] != c)
				i++;
		}
		i++;
	}
	return (0);
}

int	check_pipe_syntax(t_data *data)
{
	char	*input;
	int		i;

	input = data->input;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == '\t' || input[i] == '\n' || input[i] == '\v' \
		|| input[i] == '\f' || input[i] == '\r' || input[i] == ' ')
			i++;
		if (input[i] == '|' \
		|| (ft_strchr(input, '|') != NULL && input[i] == '\0'))
			return (write_syntax_error("|"));
		else if (input[i] != '\0')
		{
			while (input[i] != '|' && input[i] != '\0')
				i++;
			if (input[i] == '|' && (input[i + 1] == '|' \
			|| input[i + 1] == '\0'))
				return (write_syntax_error("|"));
		}
		if (input[i] != '\0')
			i++;
	}
	return (0);
}
