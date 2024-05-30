/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 10:33:52 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_marker;

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

int	parse_com_and_args(t_token *new, char **tokenarr, int exit_status)
{
	int	j;
	int i;

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


void	free_token(t_token *token)
{
	if (token->args != NULL)
		ft_free_double(token->args);
	if (token->com != NULL)
		free(token->com);
	free(token);
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

int explore_tokenarr(t_token *new, char **tokenarr)
{
	int i;
	int exit_status;

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

int	add_new_token(t_data *data, char **tokenarr)
{
	t_token	*new;
	int exit_status;
	int i;

	i = 0;
	exit_status = 0;
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (write_sys_error("malloc error"));
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
	if (new->comcount > 0)
	{
		exit_status = parse_com_and_args(new, tokenarr, exit_status);
		if (exit_status != 0)
			return (exit_status);
	}
	ft_lstadd_back_ms(&data->tokens, new);
	exit_status = add_files_to_token(new, tokenarr);
	if (exit_status != 0)
		return (exit_status);
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
		&& str[i - 1] != '>')))
			result[j++] = ' ';
		result[j++] = str[i++];
	}
	result[j] = '\0';
	ret = ft_strdup(result);
	if (!ret)
		return(NULL);
	return (ret);
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
		return (exit_status);
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
		{
			ft_free_double(inputarr);
			return (write_sys_error("malloc error"));
		}
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

void	copy_cur_dir_from_data(t_data *data, char *parsing_cur_dir)
{
	int	i;

	i = -1;
	while (data->current_directory[++i] != '\0')
		parsing_cur_dir[i] = data->current_directory[i];
	parsing_cur_dir[i] = '\0';
}

int	parsing(void)
{
	t_data		*data;
	t_env_lst 	*env_lst;
	extern char **environ;
	int			exit_status;
	char		parsing_cur_dir[256];

	exit_status = 0;
	get_parsing_cur_dir(parsing_cur_dir);
	if (parsing_cur_dir[0] == 0)
		write_sys_error("malloc error; working directory storage set up failed"); // should we exit...?
	g_signal_marker = 0;
	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		return (write_sys_error("environmental variable set up unsuccessful"));
	while (1)
	{
		alter_termios(0);
		process_signal_main();
		data = (t_data *)malloc(sizeof (t_data));
		if (!data)
			return (write_sys_error("malloc failed"));
		init_data(data, exit_status, parsing_cur_dir);
		data->input = readline("minishell-1.1$: ");

		if (g_signal_marker == 2)
		{
			data->prev_exit_status = 1;
			g_signal_marker = 0;
		}
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
		copy_cur_dir_from_data(data, parsing_cur_dir);
		ft_free_data(data, 0);
	}
	free_env_lst(env_lst);
	//ft_free_data(data, 0);
	//rl_clear_history();
	return (exit_status);
}
