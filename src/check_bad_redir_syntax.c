/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bad_redir_syntax.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:36:33 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 12:36:36 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	travel_to_next_quot(char *str, int j, char quot)
{
	j++;
	while (str[j] != quot)
		j++;
	j++;
	return (j);
}

int	check_outredir_syntax(char *str, int j)
{
	j++;
	if (str[j] == '<')
	{
		if (str[j + 1] == '>')
			write_syntax_error("<<");
		else
			write_syntax_error("<");
		return (-1);
	}
	else if (str[j] == '>' && str[j + 1] == '<')
	{
		if (str[j + 2] == '>')
			write_syntax_error("<<");
		else
			write_syntax_error("<");
		return (-1);
	}
	else if (str[j] == '>' && str[j + 1] == '>')
	{
		write_syntax_error(">");
		return (-1);
	}
	while (str[j] == '<' || str[j] == '>' || str[j] == '\0')
		j++;
	return (j);
}

int	check_inredir_syntax(char *str, int j)
{
	j++;
	if (str[j] == '>')
	{
		if (str[j + 1] == '>')
			write_syntax_error(">");
		else
			write_syntax_error("newline");
		return (-1);
	}
	else if (str[j] == '<' && str[j + 1] == '>')
	{
		if (str[j + 2] == '>')
			write_syntax_error(">>");
		else
			write_syntax_error(">");
		return (-1);
	}
	else if (str[j] == '<' && str[j + 1] == '<')
	{
		write_syntax_error("newline");
		return (-1);
	}
	while (str[j] == '<' || str[j] == '>' || str[j] == '\0')
		j++;
	return (j);
}

int	check_bad_redir_syntax(char **tokenarr, int i)
{
	int	j;

	j = 0;
	while (tokenarr[++i] != NULL)
	{
		j = 0;
		while (tokenarr[i][j] != '\0')
		{
			if (tokenarr[i][j] == '\"' || tokenarr[i][j] == '\'')
				j = travel_to_next_quot(tokenarr[i], j, tokenarr[i][j]);
			else if (tokenarr[i][j] == '<')
				j = check_inredir_syntax(tokenarr[i], j);
			else if (tokenarr[i][j] == '>')
				j = check_outredir_syntax(tokenarr[i], j);
			else
				j++;
			if (j == -1)
				return (258);
		}
	}
	return (0);
}
