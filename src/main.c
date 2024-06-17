/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:20:46 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 13:20:52 by pkangas          ###   ########.fr       */
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

int	update_prev_exit_status(t_data *data, int exit_status)
{
	if (g_signal_marker == 2)
	{
		data->prev_exit_status = 1;
		exit_status = 1;
		g_signal_marker = 0;
	}
	set_signals_to_dfl_or_ign(0);
	return (exit_status);
}

int	set_input_and_env(t_env *env_lst, int exit_status)
{
	t_data		*data;
	char		parsing_cur_dir[256];

	if (get_parsing_cur_dir(parsing_cur_dir) == 1)
		return (write_sys_error("current dir storage set up failed"));
	g_signal_marker = 0;
	while (1)
	{
		if (alter_termios(0) == 1)
			exit_status = 1;
		process_signal_main();
		data = (t_data *)malloc(sizeof (t_data));
		if (!data)
			return (write_sys_error("malloc failed"));
		init_data(data, exit_status, parsing_cur_dir);
		errno = 0;
		data->input = readline("minishell-1.1$: ");
		if (errno == ENOMEM)
			write_sys_error("malloc failed in readline");
		exit_status = update_prev_exit_status(data, exit_status);
		exit_status = parsing(data, env_lst, exit_status);
		copy_cur_dir_from_data(data, parsing_cur_dir);
		ft_free_data(data, 0);
	}
	return (exit_status);
}

int	main(void)
{
	extern char	**environ;
	t_env		*env_lst;
	int			exit_status;

	exit_status = 0;
	env_lst = save_env_list(environ);
	if (env_lst == NULL)
		return (write_sys_error("environmental variable set up unsuccessful"));
	exit_status = set_input_and_env(env_lst, exit_status);
	free_env_lst(env_lst);
	return (exit_status);
}
