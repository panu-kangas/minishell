/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:36:30 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/12 14:31:07 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*is_a_path(t_data *data, char *cmd, int *exit_status)
{
	char	*cmd_path;

	*exit_status = check_cmd_path(data, cmd);
	if (*exit_status != 0)
		return (NULL);
	cmd_path = ft_strdup(cmd);
	return (cmd_path);
}

char	*find_cmd_path(t_data *data, char *cmd, char **paths, int *exit_status)
{
	int		i;
	char	*cmd_path;

	if (cmd != NULL && ft_strchr(cmd, '/') != NULL)
		return (is_a_path(data, cmd, exit_status));
	i = -1;
	while (paths != NULL && paths[++i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (cmd_path == NULL)
		{
			write_sys_error("malloc failed");
			return (NULL);
		}
		if (access(cmd_path, F_OK) == 0)
			break ;
		free(cmd_path);
	}
	if (paths != NULL && paths[i] == NULL)
	{
		write_error(NULL, cmd, "command not found");
		*exit_status = 127;
		return (NULL);
	}
	return (cmd_path);
}

int	handle_paths(t_env *env_lst, char *cmd, char ***final_paths)
{
	char	**paths;

	if (check_empty_input(cmd) == 1)
		return (127);
	paths = NULL;
	if (check_if_var_exist(env_lst, "PATH") != NULL)
	{
		paths = get_paths(env_lst);
		if (paths == NULL)
			return (write_sys_error("malloc failed"));
	}
	if (paths == NULL && ft_strchr(cmd, '/') == NULL)
	{
		write_error(NULL, cmd, "No such file or directory");
		return (127);
	}
	*final_paths = paths;
	return (0);
}

int	handle_cmd_path(char **paths, char *cmd_path, int exit_status)
{
	if (cmd_path == NULL)
	{
		ft_free_doubleptr(paths);
		return (exit_status);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		ft_free_doubleptr(paths);
		write_error(NULL, cmd_path, "Permission denied");
		return (126);
	}
	ft_free_doubleptr(paths);
	return (0);
}

int	execute_command(char *cmd, char **e_args, t_env *env_lst, t_data *data)
{
	char	*cmd_path;
	char	**paths;
	char	**env_var_arr;
	int		exit_status;

	exit_status = handle_paths(env_lst, cmd, &paths);
	if (exit_status != 0)
		return (exit_status);
	cmd_path = find_cmd_path(data, cmd, paths, &exit_status);
	exit_status = handle_cmd_path(paths, cmd_path, exit_status);
	if (exit_status != 0)
		return (exit_status);
	env_var_arr = make_env_var_array(env_lst);
	if (env_var_arr == NULL)
	{
		ft_free_doubleptr(paths);
		free(cmd_path);
		return (write_sys_error("malloc failed"));
	}
	free_env_lst(env_lst);
	close_std_fd(data->std_fd);
	ft_free_data(data, 0);
	execve(cmd_path, e_args, env_var_arr);
	free_all_after_execve_error(cmd_path, env_var_arr);
	return (-2);
}
