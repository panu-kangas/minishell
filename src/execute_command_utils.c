/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:36:41 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 11:24:42 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_empty_input(char *cmd)
{
	if (cmd[0] == '\0')
		return (write_error(NULL, "\0", "command not found"));
	else
		return (0);
}

int	check_for_directory(char *cur_dir)
{
	struct stat	statbuf;

	if (stat(cur_dir, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 0)
			return (0);
		else
			return (1);
	}
	return (1);
}

static int	get_paths_check(t_data *data, char ***s_cmd, char **dir, char *cmd)
{
	*s_cmd = ft_pipex_split(cmd, '/');
	if (*s_cmd == NULL)
		return (write_sys_error("malloc failed"));
	if (cmd != NULL && cmd[0] == '/')
		*dir = ft_strdup("/");
	else
		*dir = ft_strdup(data->current_directory);
	if (*dir == NULL)
	{
		ft_free_doubleptr(*s_cmd);
		return (write_sys_error("malloc failed"));
	}
	return (0);
}

int	check_for_bad_path(t_data *data, char *cmd, int i)
{
	char	**split_cmd;
	char	*cur_dir;

	if (get_paths_check(data, &split_cmd, &cur_dir, cmd) == 1)
		return (1);
	while (split_cmd[++i] != NULL)
	{
		cur_dir = update_cur_dir(cur_dir, split_cmd[i]);
		if (cur_dir == NULL)
		{
			ft_free_doubleptr(split_cmd);
			return (write_sys_error("malloc failed"));
		}
		if (check_for_directory(cur_dir) == 0)
			break ;
	}
	free(cur_dir);
	if (split_cmd[i] != NULL)
	{
		ft_free_doubleptr(split_cmd);
		write_error(NULL, cmd, "Not a directory");
		return (126);
	}
	ft_free_doubleptr(split_cmd);
	return (0);
}

int	check_cmd_path(t_data *data, char *cmd)
{
	struct stat	statbuf;
	int			exit_status;

	if (access(cmd, F_OK == -1))
	{
		exit_status = check_for_bad_path(data, cmd, -1);
		if (exit_status != 0)
			return (exit_status);
		write_error(NULL, cmd, "No such file or directory");
		return (127);
	}
	if (stat(cmd, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 1)
		{
			write_error(NULL, cmd, "is a directory");
			return (126);
		}
	}
	if (access(cmd, X_OK) == -1)
	{
		write_error(NULL, cmd, "Permission denied");
		return (126);
	}
	return (0);
}
