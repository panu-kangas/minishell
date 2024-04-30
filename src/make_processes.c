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

int	free_close_wait(pid_t *pids, int **fd_pipes, t_data *data)
{
	int	i;
	int	stat_loc;
	int	exit_status;

	i = 0;
	exit_status = 0;
	close_all_pipes(fd_pipes, (data->proc_count - 1));
	while (i < data->proc_count)
		waitpid(pids[i++], &stat_loc, 0);  // status code needed here! Change the waitpid.
	free(pids);
	ft_free_int_doubleptr(fd_pipes);
	if (WIFEXITED(stat_loc) == 1)	// we also need an else statement to check if child didn't exit normally
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
	int			std_fd[2];

	if (data->tokens->args != NULL && ft_strncmp(data->tokens->args[0], "$?", 3) == 0) // ONLY FOR TESTING !!! REMOVE THIS!!!
	{
		ft_printf("%d\n", data->prev_exit_status);
		return (0);
	}

	if (data->tokens->next == NULL && \
	check_for_built_in(data->tokens->com) == 1)
	{
		std_fd[0] = dup(0); // error handling
		std_fd[1] = dup(1); // error handling
		exit_status = ft_redirect(data, NULL, 0);
		if (exit_status == 0)
			exit_status = handle_command(data, env_lst, 0);
		dup2(std_fd[0], 0); // error handling
		dup2(std_fd[1], 1); // error handling
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


	signal(SIGINT, SIG_IGN); // Ignore these in parent, so they can be set in child && USE SIGACTION
	signal(SIGQUIT, SIG_IGN); // Ignore these in parent, so they can be set in child && USE SIGACTION

	index = 0;
	while (index < data->proc_count)
	{
		pids[index] = fork();
		if (pids[index] < 0)
			return (fork_exit(fd_pipes, index, pids, data));
		else if (pids[index] == 0)
		{
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
	exit_status = free_close_wait(pids, fd_pipes, data);
	return (exit_status);
}
