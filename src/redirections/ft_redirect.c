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

int	ft_redirect(char *type, char *file)
{
	int	exit_code;

	exit_code = 0;
	if (type[0] == '<' && ft_strlen(type) == 1)
		exit_code = open_infile(file);
	else if (type[0] == '<' && ft_strlen(type) == 2)
		exit_code = ft_heredoc(file);
	else if (type[0] == '>' && ft_strlen(type) == 1)
		exit_code = open_outfile(file);
	else if (type[0] == '>' && ft_strlen(type) == 2)
		exit_code = open_outfile_append(file);
	return (exit_code);
}
