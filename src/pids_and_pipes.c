/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids_and_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:46:42 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 12:46:44 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_int_doubleptr(int **ptr)
{
	int	i;

	i = 0;
	if (ptr != NULL)
	{
		while (ptr[i] != NULL)
			free(ptr[i++]);
	}
	free(ptr);
}

void	init_fd_pipes(int **fd_pipes, int pipe_cnt)
{
	int	i;

	i = pipe_cnt;
	while (--i > -1)
	{
		fd_pipes[i][0] = -2;
		fd_pipes[i][1] = -2;
	}
}

int	make_pipes(int **fd_pipes, int pipe_cnt)
{
	int	i;

	if (fd_pipes == NULL)
		return (0);
	init_fd_pipes(fd_pipes, pipe_cnt);
	i = 0;
	while (i < pipe_cnt)
	{
		if (pipe(fd_pipes[i]) < 0)
			return (write_sys_error("pipe failed"));
		i++;
	}
	return (0);
}

int	**get_pipes(int pipe_cnt)
{
	int	i;
	int	**fd_pipes;

	fd_pipes = malloc((pipe_cnt + 1) * sizeof(int *));
	if (fd_pipes == NULL)
		return (NULL);
	i = 0;
	while (i < pipe_cnt)
	{
		fd_pipes[i] = malloc(2 * sizeof(int));
		if (fd_pipes[i] == NULL)
		{
			ft_free_int_doubleptr(fd_pipes);
			return (NULL);
		}
		i++;
	}
	fd_pipes[i] = NULL;
	if (make_pipes(fd_pipes, pipe_cnt) == 1)
	{
		ft_free_int_doubleptr(fd_pipes);
		return (NULL);
	}
	return (fd_pipes);
}

pid_t	*get_pids(int proc_count)
{
	pid_t	*pids;

	pids = malloc(proc_count * sizeof(pid_t));
	if (pids == NULL)
		return (NULL);
	return (pids);
}
