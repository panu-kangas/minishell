#include "minishell.h"

void	close_and_init_fd(int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = -2;
}

void	close_all_pipes(int **fd_pipes, int pipe_cnt)
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
}

void	free_close_wait(pid_t *pids, int **fd_pipes, t_data *data)
{
	int	i;

	i = 0;
	close_all_pipes(fd_pipes, (data->proc_count - 1));
	while (i < data->proc_count)
		waitpid(pids[i++], NULL, 0);  // status code needed here! Change the waitpid.
	free(pids);
	ft_free_int_doubleptr(fd_pipes);
}

int	fork_exit(int **fd_pipes, int index, pid_t *pids, t_data *data)
{
	close_all_pipes(fd_pipes, (data->proc_count - 1));
	while (--index != -1)
		waitpid(pids[index], NULL, 0); // status code needed here! Change the waitpid.
	free(pids);
	ft_free_int_doubleptr(fd_pipes);
	return (write_sys_error("fork failed"));
}

int	make_processes(t_data *data, t_env_lst *env_lst)
{
	pid_t		*pids;
	int			**fd_pipes;
	int			index;
	int			exit_status;

	if (data->tokens->next == NULL && \
	check_for_built_in(data->tokens->com) == 1)
	{
		exit_status = ft_redirect(data, NULL, 0);
		if (exit_status == 0)
			exit_status = handle_command(data, env_lst, 0); // CHECK NOTE IN THE END
		return (exit_status);
	}

	pids = get_pids(data->proc_count);
	if (pids == NULL)
		return (write_sys_error("malloc failed"));

	fd_pipes = get_pipes(data->proc_count - 1);
	if (fd_pipes == NULL)
	{
		free(pids);
		return (write_sys_error("malloc failed"));
	}

	// it's still a bit unclear if THIS would be the best place for heredoc_handling?
	// I did it in pipex, but now a have a better system for redirections...
	// I use them only inside processes. Let's see if it works, if not, we add hd-check here!

	index = 0;
	while (index < data->proc_count)
	{
		pids[index] = fork();
		if (pids[index] < 0)
			return (fork_exit(fd_pipes, index, pids, data));
		else if (pids[index] == 0)
		{
			free(pids);
			exit_status = ft_redirect(data, fd_pipes, index); // exit status needs to be checked!
			if (exit_status == 0)
				exit_status = handle_command(data, env_lst, index); // CHECK NOTE IN THE END
			free_env_lst(env_lst);
			ft_free_data(data, 0);
			exit(exit_status);
		}
		index++;
	}
	free_close_wait(pids, fd_pipes, data);
	return (0);
}

// NOTE REGARDING HANDLE_COMMAND:
// We need to get the error status that the function returns. But if I remember correctly, it is only taken from the last pipe-process
// So we need to investigate this and then alter the code based on that.