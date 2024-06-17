/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:35:51 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 12:03:41 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_bad_filename(t_data *data, char *filename, t_file *file)
{
	if (filename != NULL && filename[0] == '\0')
		return (write_error(NULL, "", "No such file or directory"));
	if (filename != NULL && ft_strchr(filename, '$') != NULL \
	&& ft_strlen(filename) == 1)
		return (0);
	if (filename != NULL && ft_strchr(filename, '$') != NULL \
	&& check_amb_status(file, filename) == 1)
		return (write_amb_error(filename));
	if (ft_strchr(filename, '/') != NULL \
	&& check_for_bad_path(data, filename, -1) != 0)
		return (1);
	if (access(filename, F_OK) == -1 && ft_strchr(filename, '/') != NULL)
		return (write_error(NULL, filename, "No such file or directory"));
	return (0);
}

int	ft_just_create_file(t_data *data, char *filename, t_file *file)
{
	int			file_fd;
	struct stat	statbuf;

	if (check_for_bad_filename(data, filename, file) == 1)
		return (1);
	if (stat(filename, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 1)
			return (write_error(NULL, filename, "Is a directory"));
		if (access(filename, W_OK) == -1)
			return (write_error(NULL, filename, "Permission denied"));
	}
	file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
		return (write_sys_error("open failed"));
	close(file_fd);
	return (0);
}

int	open_outfile_append(t_data *data, char *filename, t_file *file)
{
	int			file_fd;
	struct stat	statbuf;

	if (check_for_bad_filename(data, filename, file) == 1)
		return (1);
	if (stat(filename, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 1)
			return (write_error(NULL, filename, "Is a directory"));
		if (access(filename, W_OK) == -1)
			return (write_error(NULL, filename, "Permission denied"));
	}
	file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

int	open_outfile(t_data *data, char *filename, t_file *file)
{
	int			file_fd;
	struct stat	statbuf;

	if (check_for_bad_filename(data, filename, file) == 1)
		return (1);
	if (stat(filename, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 1)
			return (write_error(NULL, filename, "Is a directory"));
		if (access(filename, W_OK) == -1)
			return (write_error(NULL, filename, "Permission denied"));
	}
	file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

int	open_infile(t_data *data, char *filename, t_file *file)
{
	int	file_fd;

	if (check_for_bad_filename(data, filename, file) == 1)
		return (1);
	if (access(filename, F_OK) == -1)
		return (write_error(NULL, filename, "No such file or directory"));
	if (access(filename, R_OK) == -1)
		return (write_error(NULL, filename, "Permission denied"));
	file_fd = open(filename, O_RDONLY);
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
