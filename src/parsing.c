/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/06 11:20:17 by tsaari           ###   ########.fr       */
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

int	check_no_filename(char **tokenarr, int i, int exit_status)
{
	if (check_redir(tokenarr[i]) == -2)
		exit_status = handle_no_file(tokenarr, i, exit_status);
	else if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 5 && tokenarr[i + 1] == NULL)
		exit_status = handle_no_file(tokenarr, i, exit_status);
	return (exit_status);
}

int	add_new_token(t_data *data, char **tokenarr, int exit_status)
{
	int		i;
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (write_sys_error("malloc failed"));
	init_token(new);
	i = 0;
	while (tokenarr[i] != NULL)
	{
		exit_status = check_no_filename(tokenarr, i, exit_status);
		if (exit_status != 0)
			return (free_token(new, exit_status));
		if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
			i += 2;
		else if (check_redir(tokenarr[i]) > 4)
			i++;
		else
		{
			i = parse_com_and_args(new, tokenarr, i);
			if (i == -1)
				return (free_token(new, 1));
		}
	}
	ft_lstadd_back_ms(&data->tokens, new);
	if ((add_files_to_token(new, tokenarr)) == -1)
		return (free_token(new, 1));// tsekkaa
	return (0);
}

static int	parse_single_token(char *str, t_data *data, int exit_status)
{
	char	**tokenarr;
//	int		i; --> are these needed?
//	int		filecount; --> are these needed?

//	i = 0; --> are these needed?
//	filecount = 0; --> are these needed?
	tokenarr = ft_pipex_split(str, ' ');
	if (!tokenarr)
		return (write_sys_error("malloc failed"));
	exit_status = add_new_token(data, tokenarr, exit_status);
	if (exit_status != 0)
	{
		ft_free_double(tokenarr);
		return (exit_status);
	}
	ft_free_double(tokenarr);
	return (0);
}

static int	parse_input(t_data *data, int exit_status)
{
	char	**inputarr;
	char	*temp;

	inputarr = ft_pipex_split(data->input, '|');
	if (!inputarr)
		return (write_sys_error("malloc failed"));
	while (inputarr[data->proc_count] != NULL)
	{
		temp = inputarr[data->proc_count];
		inputarr[data->proc_count] = ft_strtrim(temp, " ");
		if (!inputarr[data->proc_count])
		{
			ft_free_double(inputarr);
			free(temp);
			return (write_sys_error("malloc failed")); // is malloc the only thing that might fail in ft_strtrim?
		}
		exit_status = parse_single_token(inputarr[data->proc_count], data, exit_status);
		if (exit_status != 0)
		{
			ft_free_double(inputarr);
			free(temp);
			return (exit_status);
		}
		free(temp);
		data->proc_count++;
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

	exit_status = 0;
	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		return (write_sys_error("env_var malloc failed")); // is it bad that the program ends here...?
	while (1)
	{
		alter_termios(0);
		process_signal_main();
		data = (t_data *)malloc(sizeof (t_data));
		if (!data)
			return (write_sys_error("malloc failed"));
		init_data(data, exit_status);
		data->input = readline("Minishell: ");
		alter_termios(1);
		if (!data->input)
		{
			free(data);
			ft_putstr_fd("exit\n", 2); // this now appears on a new line, even though it should be on the same line as our prompt
			return (0);
		}
		if (ft_strlen(data->input) != 0)
		{
			add_history(data->input);
			exit_status = parse_input(data, 0);
			exit_status = parse_out_quotes(data, exit_status);
			if (exit_status == 0)
				exit_status = make_processes(data, env_lst); // should system errors like "malloc fail" lead to whole program's termination...?
	//		ft_lstiter_ms(data->tokens, printnode);
		}
		ft_free_data(data, 0);
	}
	free_env_lst(env_lst);
	ft_free_data(data, 0);
	return (exit_status);
}
