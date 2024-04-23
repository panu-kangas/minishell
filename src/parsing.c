/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/23 18:10:19 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_com_and_args(t_token *new, char **tokenarr, int i)
{
	int	j;

	new->com = strdup(tokenarr[i]);
	if (!new->com)
	{
		free(new->com);
		free(new);
		return (-1);
	}
	i++;
	j = i;
	while (tokenarr[i] != NULL)
	{
		if (check_redir(tokenarr[i]) >= 0)//tsekkaa taa
			break ;
		i++;
	}
	if (j <= i)
	{
		new->args = make_args_arr(tokenarr, j, i);
		if (!new->args)
			return (-1);
	}
	return (i);
}

int	free_token(t_token *token, int code)
{
	ft_free_double(token->args);
	free(token->com);
	free(token);
	return (code);
}

int	check_no_filename(char **tokenarr, int i)
{
	if (check_redir(tokenarr[i]) == -2)
		handle_no_file(tokenarr, i);
	else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 5 && tokenarr[i + 1] == NULL)
		handle_no_file(tokenarr, i);
	return (0);
}

int	add_new_token(t_data *data, char **tokenarr)
{
	int		i;
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (-1);
	init_token(new);
	i = 0;
	while (tokenarr[i] != NULL)
	{
		check_no_filename(tokenarr, i);
		if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
			i += 2;
		else if (check_redir(tokenarr[i]) > 4)
			i++;
		else
		{
			i = parse_com_and_args(new, tokenarr, i);
			if (i == -1)
				return (free_token(new, -1));
		}
	}
	ft_lstadd_back_ms(&data->tokens, new);
	if ((add_files_to_token(new, tokenarr)) == -1)
		return (free_token(new, -1));// tsekkaa
	return (0);
}

static int	parse_single_token(char *str, t_data *data)
{
	char	**tokenarr;
	int		i;
	int		filecount;

	i = 0;
	filecount = 0;
	tokenarr = ft_pipex_split(str, ' ');
	if (!tokenarr)
		return (-1);
	if (add_new_token(data, tokenarr) == -1)
	{
		ft_free_double(tokenarr);
		return (-1);
	}
	ft_free_double(tokenarr);
	return (0);
}

static int	parse_input(t_data *data)
{
	char	**inputarr;
	char	*temp;

	inputarr = ft_pipex_split(data->input, '|');
	if (!inputarr)
		return (-1);
	while (inputarr[data->pipecount] != NULL)
	{
		temp = inputarr[data->pipecount];
		inputarr[data->pipecount] = ft_strtrim(temp, " ");
		if (!inputarr[data->pipecount])
		{
			ft_free_double(inputarr);
			free(temp);
			return (-1);
		}
		if (parse_single_token(inputarr[data->pipecount], data) == -1)
		{
			ft_free_double(inputarr);
			free(temp);
			return (-1);
		}
		free(temp);
		data->pipecount++;
	}
	ft_free_double(inputarr);
	return (0);
}

int	parsing(void)
{
	t_data		*data;
	t_env_lst 	*env_lst;
	extern char **environ;
	int			exit_status;

	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		return (write_sys_error("malloc failed"));
	while (1)
	{
		data = (t_data *)malloc(sizeof (t_data));
		if (!data)
			return (-1);
		init_data(data);
		data->input = readline("Minishell: ");
		if (!data->input)
		{
			free(data);
			return (-1);
		}
		if (ft_strncmp(data->input, "exit", 5) == 0)
			break ;
		add_history(data->input);
		if (parse_input(data) == -1)
			return (ft_free_data(data, 1));
	//	FORKING FUNCTION HERE --> We don't necessary need child process for built-ins... should we still do it?
	//	ft_redirect() --> I'll add this here once the function's parameters have been fixed to match the data-struct
		exit_status = handle_command(env_lst, data->tokens->com, data->tokens->args); // simple test function for command handling.
//		ft_lstiter_ms(data->tokens, printnode);
		ft_free_data(data, 0);
	}
	ft_free_data(data, 0);
	return (exit_status);
}
