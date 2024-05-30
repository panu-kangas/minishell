/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 21:24:34 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_marker;

static int	add_new_token(t_data *data, char **tokenarr)
{
	t_token	*new;
	int exit_status;
	int i;

	i = 0;
	exit_status = 0;
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (1);
	init_token(new);
	exit_status = explore_tokenarr(new, tokenarr);
	if (exit_status != 0)
	{
		free (new);
		return (exit_status);
	}
	new->args = (char **)malloc((new->arg_count + 1) * sizeof(char *));
	while (i <= new->arg_count)
		new->args[i++] = NULL;
	exit_status = parse_com_and_args(new, tokenarr, exit_status);
	if (exit_status != 0)
		return (exit_status);
	ft_lstadd_back_ms(&data->tokens, new);
	exit_status = add_files_to_token(new, tokenarr);
	return (exit_status);
}

static int	parse_single_token(char *str, t_data *data, int exit_status)
{
	char	**tokenarr;
	char	*temp;

	temp = check_non_spaced_files(str);
	if (!temp)
		return (write_sys_error("malloc error"));
	tokenarr = ft_pipex_split(temp, ' ');
	free (temp);
	if (!tokenarr)
		return (write_sys_error("malloc error"));
	exit_status = add_new_token(data, tokenarr);
	if (exit_status != 0)
	{
		ft_free_double(tokenarr);
		return (write_sys_error("malloc error"));
	}
	ft_free_double(tokenarr);
	return (0);
}

static int	lexer_input(t_data *data, int exit_status)
{
	char	**inputarr;
	char	*temp;

	inputarr = ft_pipex_split(data->input, '|');
	if (!inputarr)
		return (write_sys_error("malloc error"));
	while (inputarr[data->proc_count] != NULL)
	{
		temp = inputarr[data->proc_count];
		inputarr[data->proc_count] = ft_strtrim(temp, " ");
		free(temp);
		if (!inputarr[data->proc_count])
			return (write_sys_error("malloc error"));
		exit_status = parse_single_token(inputarr[data->proc_count], data, exit_status);
		if (exit_status != 0)
		{
			ft_free_double(inputarr);
			return (exit_status);
		}
		data->proc_count++;
	}
	ft_free_double(inputarr);
	return (0);
}

int parsing_pipeline(t_data *data, t_env_lst *env_lst)
{
	int exit_status;

	exit_status = 0;
	if (check_quot_syntax(data) != 0 || check_pipe_syntax(data) != 0)
		return (258);
	exit_status = lexer_input(data, exit_status);
	if (exit_status != 0)
		return (exit_status); //check leaks
	exit_status = parse_expansions(data, env_lst); //this removes now also quotes
	if (exit_status != 0)
		return (exit_status); //check leaks
	return(exit_status);
}

int	parsing(t_data *data, t_env_lst *env_lst, int exit_status)
{
	alter_termios(1);
	if (!data->input)
	{
		free(data);
		ft_putstr_fd("exit\n", 2);
		return (0);
	}
	if (ft_strlen(data->input) != 0)
	{
		add_history(data->input); // error handling ??
		if (handle_only_spaces(data) == 0)
		{
			exit_status = parsing_pipeline(data, env_lst);
			//ft_lstiter_ms(data->tokens, printnode);
			if (exit_status == 0)
				exit_status = make_processes(data, env_lst); // should system errors like "malloc fail" lead to whole program's termination...?
		}
	}
	return (exit_status);
}
