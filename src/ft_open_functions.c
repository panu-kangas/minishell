#include "minishell.h"

int	check_for_bad_filename(char *file)
{
	if (file != NULL && file[0] == '\0')
		return (write_error(NULL, "", "No such file or directory"));
	if (file != NULL && file[0] == '$')
		return (write_amb_error(file));
	return (0);
}

int	ft_just_create_file(char *file)
{
	int			file_fd;
	struct stat	statbuf;

	if (file != NULL && file[0] == '$')
		return (write_amb_error(file));
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
	close(file_fd);
	return (0);
}

int	open_outfile_append(char *file)
{
	int			file_fd;
	struct stat	statbuf;

	if (check_for_bad_filename(file) == 1)
		return (1);
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
	struct stat	statbuf;

	if (check_for_bad_filename(file) == 1)
		return (1);
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

	if (file != NULL && file[0] == '$')
		return (write_amb_error(file));
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
