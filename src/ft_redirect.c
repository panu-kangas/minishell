#include "minishell.h"

// in case multiple out-redirects are made

int	ft_just_create_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
		return (write_sys_error("open failed"));
	close(file_fd);
	return (0);
}

int	open_outfile_append(char *file)
{
	int			file_fd;
	struct stat statbuf;

	if (access(file, F_OK) == 0)
	{
		if (stat(file, &statbuf) == 0)
		{
			if (S_ISDIR(statbuf.st_mode) == 1)
				return (write_error(NULL, file, "Is a directory"));
		}
		if (access(file, W_OK) == -1)
			return (write_error(NULL, file, "Permission denied"));
	}
	file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file_fd == -1)
		return (write_sys_error("open failed"));
	if (dup2(file_fd, 1) == -1)
	{
		close(file_fd);
		return (write_sys_error("dup2 failed"));
	}
	close(file_fd);
	return (0);
}

int	open_outfile(char *file)
{
	int			file_fd;
	struct stat statbuf;

	if (access(file, F_OK) == 0)
	{
		if (stat(file, &statbuf) == 0)
		{
			if (S_ISDIR(statbuf.st_mode) == 1)
				return (write_error(NULL, file, "Is a directory"));
		}
		if (access(file, W_OK) == -1)
			return (write_error(NULL, file, "Permission denied"));
	}
	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
		return (write_sys_error("open failed"));
	if (dup2(file_fd, 1) == -1)
	{
		close(file_fd);
		return (write_sys_error("dup2 failed"));
	}
	close(file_fd);
	return (0);
}


int	open_infile(char *file)
{
	int	file_fd;

	if (access(file, F_OK) == -1)
		return (write_error(NULL, file, "No such file or directory"));
	if (access(file, R_OK) == -1)
		return (write_error(NULL, file, "Permission denied"));
	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
		return (write_sys_error("open failed"));
	if (dup2(file_fd, 0) == -1)
	{
		close(file_fd);
		return (write_sys_error("dup2 failed"));
	}
	close(file_fd);
	return (0);
}




t_token	*get_cur_token(t_data *data, int index)
{
	t_token	*cur_token;

	cur_token = data->tokens;

	if (index == 0)
		return (cur_token);
	else
	{
		while (index > 0)
		{
			cur_token = cur_token->next;
			index--;
		}
		return (cur_token);
	}
}



t_file	*find_infile(t_token *cur_token)
{
	t_file	*file;
	t_file	*temp;

	file = NULL;
	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == 1)
			file = temp;
		temp = temp->next;
	}
	return (file);
}


t_file	*find_outfile(t_token *cur_token)
{
	t_file	*file;
	t_file	*temp;

	file = NULL;
	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			file = temp;
		temp = temp->next;
	}
	return (file);
}

int	dup_pipe_out(int **fd_pipes, int index)
{
	int	out_fd;

	out_fd = fd_pipes[index][1];
	if (dup2(out_fd, 1) == -1)
		return (write_sys_error("dup2 failed"));
	return (0);
}

int	dup_pipe_in(int **fd_pipes, int index)
{
	int	in_fd;

	in_fd = fd_pipes[index - 1][0];
	if (dup2(in_fd, 0) == -1)
		return (write_sys_error("dup2 failed"));
	return (0);
}

int	ft_redirect(t_data *data, int **fd_pipes, int index)
{
	int		exit_code;
	t_token	*cur_token;
	t_file	*file;

	exit_code = 0;
	cur_token = get_cur_token(data, index);
	file = find_infile(cur_token);

	if (index != 0 && file == NULL)
		exit_code = dup_pipe_in(fd_pipes, index);
	else if (file != NULL)
	{
		if (file->is_append == -1)
			exit_code = open_infile(file->filename);
		else if (file->is_append == 1)
			exit_code = ft_heredoc(file->filename);
	}

	if (exit_code == 1)
	{
		close_all_pipes(fd_pipes, (data->proc_count - 1));
		ft_free_int_doubleptr(fd_pipes);
		return (1);
	}

	file = find_outfile(cur_token);
	
	if (index != (data->proc_count - 1) && file == NULL)
		exit_code = dup_pipe_out(fd_pipes, index);
	else if (file != NULL)
	{
		if (file->is_append == -1)
			exit_code = open_outfile(file->filename);
		else if (file->is_append == 1)
			exit_code = open_outfile_append(file->filename);
	}

	close_all_pipes(fd_pipes, (data->proc_count - 1));
	ft_free_int_doubleptr(fd_pipes);
	return (exit_code);
}
