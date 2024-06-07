/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:36:41 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 10:36:45 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all_from_process(char *cmd_path, char **e_args, char **env_var)
{
	free(cmd_path);
	ft_free_doubleptr(e_args);
	ft_free_doubleptr(env_var);
}

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

int	check_for_bad_path(t_data *data, char *cmd)
{
	char	**split_cmd;
	char	*cur_dir;
	int		i;

	// if (cmd[0] == '/'); --> add the possibility of a absolute path !!
	split_cmd = ft_pipex_split(cmd, '/');
	if (split_cmd == NULL)
		return (write_sys_error("malloc failed"));
	cur_dir = ft_strdup(data->current_directory);
	if (cur_dir == NULL)
	{
		ft_free_doubleptr(split_cmd);
		return (write_sys_error("malloc failed"));
	}
	i = -1;
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
		exit_status = check_for_bad_path(data, cmd);
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
