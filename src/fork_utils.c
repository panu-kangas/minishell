/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:15:17 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 16:54:03 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_and_init_fd(int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = -2;
}

void	close_all_pipes(t_data *data, int **fd_pipes, int pipe_cnt)
{
	int	k;

	k = 0;
	if (fd_pipes != NULL)
	{
		while (k < pipe_cnt)
		{
			close_and_init_fd(&fd_pipes[k][0]);
			close_and_init_fd(&fd_pipes[k][1]);
			k++;
		}
	}
	close_hd_pipes(data);
}

int	free_close_wait(pid_t *pids, int **fd_pipes, t_data *data, int exit_status)
{
	int	i;
	int	stat_loc;

	i = 0;
	close_all_pipes(data, fd_pipes, (data->proc_count - 1));
	while (i < data->proc_count)
		waitpid(pids[i++], &stat_loc, 0);
	free(pids);
	ft_free_int_doubleptr(fd_pipes);
	if (WIFEXITED(stat_loc) == 1)
		exit_status = WEXITSTATUS(stat_loc);
	else if (WIFSIGNALED(stat_loc) == 1)
	{
		if (WTERMSIG(stat_loc) == 2)
		{
			ft_putendl_fd("", 1);
			exit_status = 130;
		}
		else if (WTERMSIG(stat_loc) == 3)
		{
			ft_putendl_fd("Quit: 3", 1);
			exit_status = 131;
		}
	}
	return (exit_status);
}

int	fork_exit(int **fd_pipes, int index, pid_t *pids, t_data *data)
{
	close_all_pipes(data, fd_pipes, (data->proc_count - 1));
	while (--index != -1)
		waitpid(pids[index], NULL, 0);
	free(pids);
	ft_free_int_doubleptr(fd_pipes);
	return (write_sys_error("fork failed"));
}
