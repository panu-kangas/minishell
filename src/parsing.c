/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:27:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/20 15:41:57 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_marker;

int	skip_file(char **tokenarr, int i)
{
	if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) < 5)
		return (2);
	else if (check_redir(tokenarr[i]) > 4)
		return (1);
	return (0);
}

int	parse_com_and_args(t_token *new, char **tokenarr, int i)
{
	int	j;

	j = 0;
	while (tokenarr[++i] != NULL)
	{
		while (tokenarr[i] != NULL && check_redir(tokenarr[i]) != -1)
			i += skip_file(tokenarr, i);
		if (tokenarr[i] == NULL)
			break;
		if (new->com == NULL && check_redir(tokenarr[i]) == -1)
		{
			new->com = ft_strdup(tokenarr[i]);
			if (!new->com)
				return (-1);
		}
		else if (check_redir(tokenarr[i]) == -1)
		{
			new->args[j] = ft_strdup(tokenarr[i]);
			if (!new->args[j])
				return (-1);
			j++;
		}
	}
	new->args[j] = NULL;
	return (0);
}

int allocate_com_and_args(t_token *new, char **tokenarr)
{
	int i;
	int arg_count;

	arg_count = 0;
	i = -1;
	while (tokenarr[++i] != NULL)
		if (check_redir(tokenarr[i]) == -1)
			arg_count++;
	new->args = (char **)malloc((arg_count + 1) * sizeof(char *));
	if (!new->args) {
		return -1;
	}
	i = -1;
	parse_com_and_args(new, tokenarr, i);
	return 0;
}

int	free_token(t_token *token, int code)
{
	if (token->args != NULL)
		ft_free_double(token->args);
	if (token->com != NULL)
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

int	add_new_token(t_data *data, char **tokenarr)
{
	int		i;
	t_token	*new;
	int exit_status;

	exit_status = 0;
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (-1);
	init_token(new);
	i = 0;
	if (allocate_com_and_args(new, tokenarr) != 0)
		return (free_token(new, -1));
	ft_lstadd_back_ms(&data->tokens, new);
	if ((add_files_to_token(new, tokenarr)) == -1)
		return (free_token(new, -1));
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

static int	parse_single_token(char *str, t_data *data)
{
	char	**tokenarr;
	int		i;

	i = 0;
	str = check_non_spaced_files(str);
	if (!str)
		return (-1);
	tokenarr = ft_pipex_split(str, ' ');
	if (!tokenarr)
		return (-1);
	if (add_new_token(data, tokenarr) != 0)
	{
		ft_free_double(tokenarr);
		return (-1);
	}
	ft_free_double(tokenarr);
	return (0);
}

static int	lexer_input(t_data *data)
{
	char	**inputarr;
	char	*temp;

	inputarr = ft_pipex_split(data->input, '|');
	if (!inputarr)
		return (-1);
	while (inputarr[data->proc_count] != NULL)
	{
		temp = inputarr[data->proc_count];
		inputarr[data->proc_count] = ft_strtrim(temp, " ");
		free(temp);
		if (!inputarr[data->proc_count])
		{
			ft_free_double(inputarr);
			return (-1);
		}
		if (parse_single_token(inputarr[data->proc_count], data) != 0)
			return (-1);
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
	exit_status = lexer_input(data);
	if (exit_status != 0)
		return (write_sys_error("malloc failed")); //check leaks
	exit_status = parse_expansions(data, env_lst); //this removes now also quotes
	if (exit_status != 0)
		return (write_sys_error("malloc failed")); //check leaks
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
