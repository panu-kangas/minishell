/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 09:39:53 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/20 15:43:06 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_file	*add_file(char *str, int is_append, int is_infile)
{
	t_file	*new;

	new = (t_file *)malloc (sizeof(t_file));
	if (!new)
		return (NULL);
	init_file(new);
	if (!str)
	{
		new->no_filename = -1;
		return (NULL);
	}
	new->filename = ft_strdup(str);
	if (!new->filename)
	{
		free (new);
		return (NULL);
	}
	new->is_append = is_append;
	new->is_infile = is_infile;
	new->next = NULL;
	return (new);
}

int check_redir(char *str)
{
	size_t len = strlen(str);

	if (len >= 3 && (strncmp(str, ">>>", 3) == 0 || strncmp(str, "<<<", 3) == 0))
		return (-2);
	else if (len == 1 && strncmp(str, "<", 1) == 0)
		return (1);
	else if (len == 1 && strncmp(str, ">", 1) == 0)
		return (2);
	else if (len == 2 && strncmp(str, "<<", 2) == 0)
		return (3);
	else if (len == 2 && strncmp(str, ">>", 2) == 0)
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

int	handle_no_file(char **tokenarr, int i, int exit_status) // Sorry Timo for messing up with your function!! I'll fix this later! =D That is totally OK :D Timo
{
	if (check_redir(tokenarr[i]) == -2)
	{
		if (tokenarr[i][2] == '>')
		{
			ft_putendl_fd ("syntax error near unexpected token `>'", 2);
			exit_status = 258;
		}
		else
		{
			ft_putendl_fd ("syntax error near unexpected token `newline'", 2);
			exit_status = 258;
		}
	}
	else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 9 && ft_strlen(tokenarr[i]) < 3)
	{
		if (tokenarr[i + 1] == NULL)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			exit_status = 258;
		}
		else if (tokenarr[i + 1][0] == '>')
		{
			ft_putendl_fd ("syntax error near unexpected token `>'", 2);
			exit_status = 258;
		}
		else if (tokenarr[i + 1][0] == '<')
		{
			ft_putendl_fd("syntax error near unexpected token `<'", 2);
			exit_status = 258;
		}
		else if (tokenarr[i + 1][0] == '|')
		{
			ft_putendl_fd ("syntax error near unexpected token `|'", 2);
			exit_status = 258;
		}
	}
	else if (check_redir(tokenarr[i]) > 4 && check_redir(tokenarr[i]) < 9)
	{
		if (tokenarr[i + 1][2] == '>')
		{
			ft_putendl_fd ("syntax error near unexpected token `>'", 2);
			exit_status = 258;
		}
		else if (tokenarr[i + 1][2] == '<')
		{
			ft_putendl_fd ("syntax error near unexpected token `<'", 2);
			exit_status = 258;
		}
	}
	// Is exit status always 258? We need to investigate! >> I think it is.
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
			return (-1);
		ft_lstadd_file_ms(&new->files, temp);
		i++;
	}
	return (0);
}

char	**make_args_arr(char **tokenarr, int j, int i)
{
	char	**args;
	int		k;

	int arraysize = i - j + 2;
	args = (char**)malloc(sizeof(char*) * arraysize);
	if (args == NULL)
		return (NULL);
	k = 0;
	while (j < i)
	{
		args[k] = ft_strdup(tokenarr[j]);
		if (args[k] == NULL)
		{
			ft_free_double(args);
			return (NULL);
		}
		j++;
		k++;
	}
	args[k] = 0;
	return (args);
}
