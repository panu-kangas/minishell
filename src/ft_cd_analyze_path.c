/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_analyze_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:20:34 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 13:20:36 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_valid_path(char *path)
{
	int		valid_path_flag;

	valid_path_flag = check_valid_path(path);
	if (valid_path_flag == 0)
		return (0);
	else if (valid_path_flag == 2)
		return (write_error("cd", path, "Not a directory"));
	else if (valid_path_flag == 3)
		return (write_error("cd", path, "Permission denied"));
	return (0);
}

int	get_cur_dir_flag(t_data *data)
{
	char	**split_cur_dir;
	int		cur_dir_flag;

	split_cur_dir = ft_pipex_split(data->current_directory, '/');
	if (split_cur_dir == NULL)
	{
		write_sys_error("malloc failed");
		return (-1);
	}
	cur_dir_flag = check_cur_dir_permissions(split_cur_dir);
	if (cur_dir_flag != -2)
		ft_free_doubleptr(split_cur_dir);
	if (cur_dir_flag < 0)
	{
		write_sys_error("malloc failed");
		return (-1);
	}
	return (cur_dir_flag);
}

int	try_to_change_dir(char *path, int cur_dir_flag)
{
	if (is_relative_path(path) == 2 && cur_dir_flag == 1)
		return (write_error("cd", path, "No such file or directory"));
	else if (chdir(path) != 0)
	{
		if (cur_dir_flag == 1)
			return (write_error("cd", path, "No such file or directory"));
		else if (cur_dir_flag == 3)
			return (write_error("cd", path, "Permission denied"));
	}
	return (-3);
}

int	break_and_test_path(t_data *data, char *path)
{
	int		path_flag;
	int		test_flag;
	char	**split_path;

	split_path = ft_pipex_split(path, '/');
	if (split_path == NULL)
		return (write_sys_error("malloc failed"));
	if (path[0] == '/')
		split_path[0] = add_slash_to_first_path(split_path[0]);
	if (split_path[0] == NULL)
		return (write_sys_error("malloc failed"));
	test_flag = test_whole_path(split_path, &path_flag, -1);
	ft_free_doubleptr(split_path);
	if (test_flag == 1)
	{
		if (chdir(data->current_directory) != 0)
			return (write_sys_error("chdir failed"));
		if (path_flag == ENOENT)
			return (write_error("cd", path, "No such file or directory"));
		else if (path_flag == EACCES)
			return (write_error("cd", path, "Permission denied"));
		else if (path_flag == ENOTDIR)
			return (write_error("cd", path, "Not a directory"));
	}
	return (-3);
}

int	analyze_path(char *path, t_data *data)
{
	int		cur_dir_flag;
	int		is_relative;
	int		parent_dir_flag;

	if (handle_valid_path(path) == 1)
		return (1);
	cur_dir_flag = get_cur_dir_flag(data);
	if (cur_dir_flag < 0)
		return (1);
	is_relative = is_relative_path(path);
	parent_dir_flag = check_parent_dir_permissions(data->current_directory);
	if (is_relative == 1 && cur_dir_flag == 3 && parent_dir_flag == 0)
		return (write_error("cd", path, "Permission denied"));
	else if (is_relative == 2 && cur_dir_flag == 3 && parent_dir_flag == 0)
		return (-2);
	else if (is_relative == 2 && cur_dir_flag == 1 && parent_dir_flag == 0)
		return (-2);
	else if (is_relative >= 6 && cur_dir_flag == 1)
		return (write_error("cd", path, "No such file or directory"));
	else if (is_relative != 0 && (cur_dir_flag == 3 || cur_dir_flag == 1))
		return (try_to_change_dir(path, cur_dir_flag));
	else if (cur_dir_flag == 0 || is_relative == 0)
		return (break_and_test_path(data, path));
	return (-1);
}
