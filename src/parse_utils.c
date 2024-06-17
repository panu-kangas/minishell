/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 09:39:53 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/17 10:32:56 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redir(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len >= 3 && (ft_strncmp(str, ">>>", 3) == 0 || \
	ft_strncmp(str, "<<<", 3) == 0))
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

int	add_files_to_token(t_token *new, char **tokenarr, int i)
{
	t_file	*temp;

	i = -1;
	while (tokenarr[++i] != NULL)
	{
		if (check_redir(tokenarr[i]) < 0)
		{
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
	}
	return (0);
}

int	ft_char_counter(char *str, char c)
{
	int	i;
	int	ctr;

	if (!str)
		return (0);
	ctr = 0;
	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == c && (ft_isalnum(str[i + 1]) == 1 || str[i + 1] == '_'))
			ctr++;
		i++;
	}
	return (ctr);
}
