/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/22 10:54:01 by tsaari           ###   ########.fr       */
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
		if (check_redir(tokenarr[i]) != -1)
			i += skip_file(tokenarr, i);
		else if (tokenarr[i] != NULL && new->com == NULL && check_redir(tokenarr[i]) == -1)
		{
			new->com = ft_strdup(tokenarr[i]);
			if (!new->com)
				return (write_sys_error("malloc error"));
			i++;
		}
		else if (tokenarr[i] != NULL && check_redir(tokenarr[i]) == -1 && new->arg_count > 0)
		{
			new->args[j] = ft_strdup(tokenarr[i]);
			if (!new->args[j])
				return (write_sys_error("malloc error"));
			j++;
			i++;
		}
		else
			i++;
	}
	new->args[j] = NULL;
	j = 0;
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
		if (check_redir(tokenarr[i]) != -1)
		{
			i += skip_file(tokenarr, i);
			new->filecount++;
		}
		else if (check_redir(tokenarr[i]) == -1)
		{
			if (new->comcount == 0)
				new->comcount++;
			else
				new->arg_count++;
			i++;
		}
		else
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
	exit_status = explore_tokenarr(new, tokenarr) != 0;
	if (exit_status != 0)
	{
		free (new);
		return (exit_status);
	}
	new->args = (char **)malloc((new->arg_count + 1) * sizeof(char *));
	while (i <= new->arg_count)
	{
		new->args[i] = NULL;
		i++;
	}
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
	char *result;
	char *ret;

	j = 0;
	i = 0;
	result = (char *)malloc(strlen(str) * 2);
	if (!result)
		return NULL;
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
	free (result);
	if (!ret)
		return(NULL);
	return (ret);
}

static int	parse_single_token(char *str, t_data *data, int exit_status)
{
	char	**tokenarr;
	int		i;
	(void)data;

	i = 0;
	str = check_non_spaced_files(str);
	if (!str)
		return (write_sys_error("malloc error"));
	tokenarr = ft_pipex_split(str, ' ');
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
		exit_status =parse_single_token(inputarr[data->proc_count], data, exit_status);
		if (exit_status != 0)
			return (exit_status);
		data->proc_count++;
	}
	ft_free_double(inputarr);
	return (0);
}

int parsing_pipeline(t_data *data, t_env_lst *env_lst)
{
	(void)env_lst;
	int exit_status;

	exit_status = 0;
	if (handle_only_spaces(data) != 0)
		return(0);
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
	parsing_cur_dir[0] = '\0';
	g_signal_marker = 0;
	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		write_sys_error("setting up env_var not successful (malloc failure)"); // maybe exit here ??
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
			ft_putstr_fd("exit\n", 2); // this now appears on a new line, even though it should be on the same line as our prompt
			return (0);
		}
		if (ft_strlen(data->input) != 0)
		{
			add_history(data->input); // error handling ??
			exit_status = parsing_pipeline(data, env_lst);
			// ft_lstiter_ms(data->tokens, printnode);
			if (exit_status == 0 && data->special_case != 0)
				exit_status = make_processes(data, env_lst); // should system errors like "malloc fail" lead to whole program's termination...?
		}
		copy_cur_dir_from_data(data, parsing_cur_dir);
		ft_free_data(data, 0);
	}
	free_env_lst(env_lst);
	//ft_free_data(data, 0);
	//rl_clear_history();
	return (exit_status);
}
