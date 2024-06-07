/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_processes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:10:26 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 17:27:58 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	store_stdin_stdout(int *std_fd)
{
	std_fd[0] = dup(0);
	if (std_fd[0] == -1)
		return (write_sys_error("dup failed"));
	std_fd[1] = dup(1);
	if (std_fd[1] == -1)
		return (write_sys_error("dup failed"));
	return (0);
}

int	handle_heredoc(t_data *data, t_env *env_lst, int *std_fd, int *e_stat)
{
	signal(SIGINT, &sig_handler_hd);
	signal(SIGQUIT, SIG_IGN);
	*e_stat = process_heredoc(data, env_lst, 0);
	if (*e_stat != 0)
	{
		if (g_signal_marker == 2)
		{
			if (dup2(std_fd[0], 0) < 0)
				return (write_sys_error("dup2 failed"));
			g_signal_marker = 0;
		}
		return (*e_stat);
	}
	return (0);
}

int	is_builtin(t_data *data, t_env *env_lst, int *std_fd, int *e_stat)
{
	int	flag;

	flag = 0;
	if (data->tokens->next == NULL && \
	check_for_built_in(data->tokens->com) == 1)
	{
		flag = 1;
		*e_stat = ft_redirect(data, NULL, 0);
		if (*e_stat == 0)
			*e_stat = handle_command(data, env_lst, 0, 1);
		if (dup2(std_fd[0], 0) == -1)
			return (write_sys_error("dup2 failed here"));
		if (dup2(std_fd[1], 1) == -1)
			return (write_sys_error("dup2 failed here"));
	}
	return (flag);
}
