/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 09:39:53 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 20:38:53 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redir(char *str)
{
	size_t	len;

	len = ft_strlen(str);

	if (len >= 3 && (ft_strncmp(str, ">>>", 3) == 0 || ft_strncmp(str, "<<<", 3) == 0))
		return (-2);
	else if (len == 1 && ft_strncmp(str, "<", 1) == 0)
		return (1);
	else if (len == 1 && ft_strncmp(str, ">", 1) == 0)
		return (2);
	else if (len == 2 && ft_strncmp(str, "<<", 2) == 0)
		return (3);
	else if (len == 2 && ft_strncmp(str, ">>", 2) == 0)
		return (4);
	else if (len >= 2 && str[0] == '<' && str[1] == '<')
		return (5);
	else if (len >= 2 && str[0] == '>' && str[1] == '>')
		return (6);
	else if (len >= 1 && str[0] == '<')
		return (7);
	else if (len >= 1 && str[0] == '>')
		return (8);
	else
		return (-1);
}

int	handle_no_file(char **tokenarr, int i, int exit_status)
{
	if (check_redir(tokenarr[i]) == -2)
	{
		if (tokenarr[i][2] == '>')
			return (write_syntax_error(">"));
		else
			return (write_syntax_error("newline"));
	}
	else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 9 && ft_strlen(tokenarr[i]) < 3)
	{
		if (tokenarr[i + 1] == NULL)
			return (write_syntax_error("newline"));
		else if (tokenarr[i + 1][0] == '>')
			return (write_syntax_error(">"));
		else if (tokenarr[i + 1][0] == '<')
			return (write_syntax_error("<"));
		else if (tokenarr[i + 1][0] == '|')
			return (write_syntax_error("|"));
	}
	else if (check_redir(tokenarr[i]) > 4 && check_redir(tokenarr[i]) < 9)
	{
		if (tokenarr[i + 1][2] == '>')
			return (write_syntax_error(">"));
		else if (tokenarr[i + 1][2] == '<')
			return (write_syntax_error("<"));
	}
	return (exit_status);
}

static t_file	*add_file_single(t_file *temp, char **tokenarr, int i)
{
	if (check_redir(tokenarr[i]) == 1)
		temp = add_file(tokenarr[i + 1], -1, 1);
	else if (check_redir(tokenarr[i]) == 2)
		temp = add_file(tokenarr[i + 1], -1, -1);
	else if (check_redir(tokenarr[i]) == 3)
		temp = add_file(tokenarr[i + 1], 1, 1);
	else if (check_redir(tokenarr[i]) == 4)
		temp = add_file(tokenarr[i + 1], 1, -1);
	if (!temp)
		return (NULL);
	return (temp);
}

int	add_files_to_token(t_token *new, char **tokenarr)
{
	int		i;
	t_file	*temp;

	i = 0;
	while (tokenarr[i] != NULL)
	{
		if (check_redir(tokenarr[i]) < 0)
		{
			i++;
			continue ;
		}
		else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
			temp = add_file_single(temp, tokenarr, i);
		else if (check_redir(tokenarr[i]) == 5)
			temp = add_file(&tokenarr[i][2], 1, 1);
		else if (check_redir(tokenarr[i]) == 6)
			temp = add_file(&tokenarr[i][2], 1, -1);
		else if (check_redir(tokenarr[i]) == 7)
			temp = add_file(&tokenarr[i][1], -1, 1);
		else if (check_redir(tokenarr[i]) == 8)
			temp = add_file(&tokenarr[i][1], -1, -1);
		if (!temp)
			return (1);
		ft_lstadd_file_ms(&new->files, temp);
		i++;
	}
	return (0);
}
