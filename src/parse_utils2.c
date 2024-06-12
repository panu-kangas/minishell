/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:44:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/12 13:46:42 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	skip_file(char **tokenarr, int i)
{
	if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
	{
		if (tokenarr[i + 1] == NULL)
			return (1);
		return (2);
	}
	else if (check_redir(tokenarr[i]) > 4)
	{
		return (1);
	}
	return (0);
}

int	check_no_filename(char **tokenarr, int i, int exit_status)
{
	exit_status = check_bad_redir_syntax(tokenarr, i);
	while (tokenarr[++i] != 0 && exit_status == 0)
	{
		if (check_redir(tokenarr[i]) == -2 && tokenarr[i][0] == '>')
		{
			if (tokenarr[i][3] != '>')
				return (write_syntax_error(">"));
			else
				return (write_syntax_error(">>"));
		}
		else if (check_redir(tokenarr[i]) == -2 && tokenarr[i][0] == '<')
		{
			if (tokenarr[i][3] != '<')
				return (write_syntax_error("<"));
			else
				return (write_syntax_error("<<"));
		}
		else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
		{
			exit_status = handle_no_file(tokenarr, i, exit_status);
			if (exit_status != 0)
				return (exit_status);
		}
	}
	return (exit_status);
}

int	explore_tokenarr(t_token *new, char **tokenarr)
{
	int	i;

	i = 0;
	while (tokenarr[i] != NULL)
	{
		if (check_redir(tokenarr[i]) > 0)
		{
			i += skip_file(tokenarr, i) - 1;
			new->filecount++;
		}
		else if (check_redir(tokenarr[i]) == -1)
		{
			if (new->comcount == 0)
				new->comcount++;
			else
				new->arg_count++;
		}
		i++;
	}
	return (0);
}

char	*check_non_spaced_files(char *str)
{
	int		i;
	int		j;
	char	*result;
	char	*ret;

	result = malloc((sizeof(char) * ft_strlen(str) * 2) + 1);
	j = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] == '<' || str[i] == '>') && \
		(i == 0 || (str[i - 1] != ' ' && str[i - 1] != '<' \
		&& str[i - 1] != '>' && str[i - 1] != '"' && str[i - 1] != '\'')))
			result[j++] = ' ';
		result[j++] = str[i++];
	}
	result[j] = '\0';
	ret = ft_strdup(result);
	free (result);
	if (!ret)
		return (NULL);
	return (ret);
}

int	parse_com_and_args(t_token *new, char **tokenarr, int exit_status)
{
	int	j;
	int	i;

	i = -1;
	j = 0;
	while (tokenarr[++i] != NULL)
	{
		if (check_redir(tokenarr[i]) > 0)
			i += skip_file(tokenarr, i) - 1;
		else if (new->com == NULL && check_redir(tokenarr[i]) == -1)
		{
			new->com = ft_strdup(tokenarr[i]);
			if (!new->com)
				return (write_sys_error("malloc error"));
		}
		else if (check_redir(tokenarr[i]) == -1 && new->arg_count > 0)
		{
			new->args[j] = ft_strdup(tokenarr[i]);
			if (!new->args[j++])
				return (write_sys_error("malloc error"));
		}
	}
	new->args[j] = NULL;
	return (exit_status);
}
