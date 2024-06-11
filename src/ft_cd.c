/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:34:19 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 13:11:03 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_dir_to_home(t_env *home, t_data *data)
{
	int	flag;

	flag = -1;
	if (home->value != NULL)
		flag = analyze_path(home->value, data);
	if (flag >= 0)
		return (flag);
	else
		flag = chdir(home->value);
	if (flag == 0)
		return (-1);
	else
		return (1);
}

int	get_absolute_parent_path(t_data *data, t_token *token)
{
	int		i;
	char	*path;

	free(token->args[0]);
	token->args[0] = ft_strdup(data->current_directory);
	if (token->args[0] == NULL)
		return (-1);
	path = token->args[0];
	i = ft_strlen(path) - 1;
	while (i > 0)
	{
		if (path[i] == '/')
		{
			if (path[i + 1] != '\0' && path[i + 1] != '/')
				break ;
		}
		i--;
	}
	if (i > 0)
		path[i] = '\0';
	return (0);
}

int	handle_cur_dir(t_data *data, t_env *env_lst, char *path)
{
	char		*cur_dir;

	errno = 0;
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL && errno != ENOMEM)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		write_sys_error("getcwd: cannot access parent directories");
		add_path_to_data_cur_dir(data, path);
	}
	else if (cur_dir == NULL && errno == ENOMEM)
		return (write_sys_error("malloc failed"));
	else
		copy_cur_dir_to_data(data, cur_dir);
	if (update_pwd_env_var(data, env_lst, cur_dir) == 1)
	{
		free(cur_dir);
		return (1);
	}
	free(cur_dir);
	return (0);
}

int	change_directory(t_data *data, t_env *env_lst, char *path, int flag)
{
	t_env	*home;

	home = check_if_var_exist(env_lst, "HOME");
	if (path == NULL && home == NULL)
		return (write_error(NULL, "cd", "HOME not set"));
	if (path == NULL)
	{
		flag = change_dir_to_home(home, data);
		if (flag >= 0)
			return (flag);
	}
	else if (flag != -3)
	{
		flag = chdir(path);
		if (flag == -1)
			return (write_sys_error("chdir failed"));
	}
	return (0);
}

int	ft_cd(t_data *data, t_env *env_lst, t_token *token)
{
	int		flag;
	char	*path;

	flag = -1;
	path = token->args[0];
	if (path != NULL && path[0] == '\0')
		return (0);
	if (ft_strlen(path) >= 256)
		return (write_error("cd", path, "File name too long"));
	if (path != NULL)
		flag = analyze_path(path, data);
	if (flag >= 0)
		return (flag);
	else if (flag == -2)
	{
		if (get_absolute_parent_path(data, token) == -1)
			return (write_sys_error("malloc failed"));
		path = token->args[0];
	}
	flag = change_directory(data, env_lst, path, flag);
	if (flag == 1)
		return (1);
	if (handle_cur_dir(data, env_lst, path) == 1)
		return (1);
	return (0);
}
