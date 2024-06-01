/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:10:39 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 16:17:52 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	enter_fork_loop(t_data *data, t_env_lst *env_lst, int *pids, int **fd_pipes)
{
	int	exit_status;
	int	index;

	exit_status = 0;
	index = 0;
	while (index < data->proc_count)
	{
		pids[index] = fork();
		if (pids[index] < 0)
			return (fork_exit(fd_pipes, index, pids, data));
		else if (pids[index] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			free(pids);
			exit_status = ft_redirect(data, fd_pipes, index);
			if (exit_status == 0)
				exit_status = handle_command(data, env_lst, index);
			free_env_lst(env_lst);
			ft_free_data(data, 0);
			exit(exit_status);
		}
		index++;
	}
	return (0);
}

int	create_processes(t_data *data, t_env_lst *env_lst)
{
	pid_t	*pids;
	int		**fd_pipes;

	pids = get_pids(data->proc_count);
	if (pids == NULL)
		return (write_sys_error("malloc failed"));
	fd_pipes = get_pipes(data->proc_count - 1);
	if (fd_pipes == NULL)
	{
		free(pids);
		return (write_sys_error("malloc failed"));
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (enter_fork_loop(data, env_lst, pids, fd_pipes) == 1)
		return (1);
	return (free_close_wait(pids, fd_pipes, data, 0));
}

int	make_processes(t_data *data, t_env_lst *env_lst)
{
	int			exit_status;
	int			std_fd[2];

	exit_status = 0;
	std_fd[0] = dup(0);
	if (std_fd[0] == -1)
		return (write_sys_error("dup failed"));
	std_fd[1] = dup(1);
	if (std_fd[1] == -1)
		return (write_sys_error("dup failed"));
	if (handle_heredoc(data, env_lst, std_fd, &exit_status) != 0)
		return (exit_status);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (data->tokens->com != NULL && ft_strchr(data->tokens->com, '=') != NULL \
	&& data->tokens->next == NULL)
	{
		exit_status = handle_command(data, env_lst, 0);
		if (exit_status != 2)
			return (exit_status);
	}
	if (is_builtin(data, env_lst, std_fd, &exit_status) == 1)
		return (exit_status);
	exit_status = create_processes(data, env_lst);
	return (exit_status);
}
