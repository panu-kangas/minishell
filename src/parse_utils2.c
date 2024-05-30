/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:44:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 21:24:22 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	skip_file(char **tokenarr, int i)
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

static int	check_no_filename(char **tokenarr, int i, int exit_status)
{
	if (check_redir(tokenarr[i]) == -2)
		exit_status = handle_no_file(tokenarr, i, exit_status);
	else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 5 && tokenarr[i + 1] == NULL)
		exit_status = handle_no_file(tokenarr, i, exit_status);
	return (exit_status);
}

int	explore_tokenarr(t_token *new, char **tokenarr)
{
	int	i;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (tokenarr[i] != NULL)
	{
		exit_status = check_no_filename(tokenarr, i, exit_status);
		if (exit_status != 0)
			return (exit_status);
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

char *check_non_spaced_files(char *str)
{
	int i;
	int j;
	char result[strlen(str) * 2];
	char *ret;

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
	if (!ret)
		return(NULL);
	return (ret);
}

int	parse_com_and_args(t_token *new, char **tokenarr, int exit_status)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (tokenarr[i] != NULL)
	{
		if (check_redir(tokenarr[i]) > 0)
			i += skip_file(tokenarr, i) - 1;
		else if (tokenarr[i] != NULL && new->com == NULL && check_redir(tokenarr[i]) == -1)
		{
			new->com = ft_strdup(tokenarr[i]);
			if (!new->com)
				return (write_sys_error("malloc error"));
		}
		else if (tokenarr[i] != NULL && check_redir(tokenarr[i]) == -1 && new->arg_count > 0)
		{
			new->args[j] = ft_strdup(tokenarr[i]);
			if (!new->args[j++])
				return (write_sys_error("malloc error"));
		}
		i++;
	}
	new->args[j] = NULL;
	return (exit_status);
}
