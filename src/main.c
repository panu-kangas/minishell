/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:51:38 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 13:14:01 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copy_cur_dir_from_data(t_data *data, char *parsing_cur_dir)
{
	int	i;

	i = -1;
	while (data->current_directory[++i] != '\0')
		parsing_cur_dir[i] = data->current_directory[i];
	parsing_cur_dir[i] = '\0';
}

void	update_prev_exit_status(t_data *data)
{
	data->prev_exit_status = 1;
	g_signal_marker = 0;
}

int	set_input_and_env(t_env *env_lst, int exit_status)
{
	t_data		*data;
	char		parsing_cur_dir[256];

	get_parsing_cur_dir(parsing_cur_dir);
	if (parsing_cur_dir[0] == 0)
		write_sys_error("malloc error; current dir storage set up failed");
	g_signal_marker = 0;
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
			update_prev_exit_status(data);
		exit_status = parsing(data, env_lst, exit_status);
		copy_cur_dir_from_data(data, parsing_cur_dir);
		ft_free_data(data, 0);
	}
	//rl_clear_history(); //do we need this??
	return (exit_status);
}

int	main(int argc, char **argv)
{
	extern char	**environ;
	t_env	*env_lst;
	int			exit_status;

	exit_status = 0;
	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		return (write_sys_error("environmental variable set up unsuccessful"));
	if (argc != 1 || argv == NULL) // this needs to be checked
	{
		//errorhandling
		return (0);
	}
	else
		exit_status = set_input_and_env(env_lst, exit_status);
	//system("leaks executablename");
	free_env_lst(env_lst);
	return (exit_status);
}
