/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/07 16:19:10 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_marker;

static int	add_new_token(t_data *data, char **tokenarr)
{
	t_token	*new;
	int		exit_status;
	int		i;

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
	exit_status = add_files_to_token(new, tokenarr, i);
	return (exit_status);
}

static int	parse_single_token(char *str, t_data *data, int exit_status)
{
	char	**tokenarr;
	char	*temp;
	int		i;

	i = -1;
	temp = check_non_spaced_files(str);
	if (!temp)
		return (write_sys_error("malloc error"));
	tokenarr = ft_pipex_split(temp, ' ');
	free (temp);
	if (!tokenarr)
		return (write_sys_error("malloc error"));
	exit_status = check_no_filename(tokenarr, i, exit_status);
	if (exit_status != 0)
	{
		ft_free_doubleptr(tokenarr);
		return (exit_status);
	}
	exit_status = add_new_token(data, tokenarr);
	ft_free_doubleptr(tokenarr);
	if (exit_status != 0)
		return (write_sys_error("malloc error"));
	return (0);
}

static int	lexer_input(t_data *data, int e_status)
{
	char	**inparr;
	char	*temp;

	inparr = ft_pipex_split(data->input, '|');
	if (!inparr)
		return (write_sys_error("malloc error"));
	while (inparr[data->proc_count] != NULL)
	{
		temp = inparr[data->proc_count];
		inparr[data->proc_count] = ft_strtrim(temp, " ");
		free(temp);
		if (!inparr[data->proc_count])
			return (write_sys_error("malloc error"));
		e_status = parse_single_token(inparr[data->proc_count], data, e_status);
		if (e_status != 0)
		{
			ft_free_doubleptr(inparr);
			return (e_status);
		}
		data->proc_count++;
	}
	ft_free_doubleptr(inparr);
	return (0);
}

int	parsing_pipeline(t_data *data, t_env *env_lst)
{
	int	exit_status;
	int	i;

	i = 0;
	exit_status = 0;
	if (check_quot_syntax(data) != 0 || check_pipe_syntax(data, i) != 0)
		return (258);
	exit_status = lexer_input(data, exit_status);
	if (exit_status != 0)
		return (exit_status);
	exit_status = parse_expansions(data, env_lst);
	if (exit_status != 0)
		return (exit_status);
	return (exit_status);
}

int	parsing(t_data *data, t_env *env_lst, int exit_status)
{
	if (!data->input)
		free_and_exit(data, env_lst);
	if (alter_termios(1) == 1)
		return (1);
	if (ft_strlen(data->input) != 0)
	{
		add_history(data->input);
		if (handle_only_spaces(data) == 0)
		{
			exit_status = parsing_pipeline(data, env_lst);
			if (exit_status == 0)
			{
				store_stdin_stdout(data->std_fd);
				exit_status = make_processes(data, env_lst, data->std_fd);
				close_std_fd(data->std_fd);
			}
		}
	}
	return (exit_status);
}
