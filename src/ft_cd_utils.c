/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:34:00 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 11:11:09 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_cur_dir_to_data(t_data *data, char *cur_dir)
{
	int	i;

	i = -1;
	while (cur_dir[++i] != '\0')
		data->current_directory[i] = cur_dir[i];
	data->current_directory[i] = '\0';
}

void	add_path_to_data_cur_dir(t_data *data, char *path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->current_directory[i] != '\0')
		i++;
	if (i > 0 && data->current_directory[i - 1] != '/')
		data->current_directory[i++] = '/';
	while (path[j] != '\0')
		data->current_directory[i++] = path[j++];
	data->current_directory[i] = '\0';
}

static int	handle_oldpwd(t_env *env_lst, t_env *pwd)
{
	t_env	*old_pwd;

	old_pwd = check_if_var_exist(env_lst, "OLDPWD");
	if (old_pwd == NULL)
	{
		if (process_non_global_env_node(env_lst, "OLDPWD=") == 1)
			return (1);
		old_pwd = check_if_var_exist(env_lst, "OLDPWD");
	}
	free(old_pwd->value);
	if (pwd == NULL)
		old_pwd->value = ft_strdup("");
	else
		old_pwd->value = ft_strdup(pwd->value);
	if (old_pwd->value == NULL)
		return (write_sys_error("malloc failed"));
	return (0);
}

int	update_pwd_env_var(t_data *data, t_env *env_lst, char *cur_dir)
{
	t_env	*pwd;
	char	*pwd_env_var;

	if (cur_dir == NULL)
		cur_dir = data->current_directory;
	pwd = check_if_var_exist(env_lst, "PWD");
	if (handle_oldpwd(env_lst, pwd) == 1)
		return (1);
	pwd_env_var = ft_strjoin("PWD=", cur_dir);
	if (pwd_env_var == NULL)
		return (write_sys_error("malloc failed"));
	if (process_non_global_env_node(env_lst, pwd_env_var) == 1)
	{
		free(pwd_env_var);
		return (1);
	}
	free(pwd_env_var);
	return (0);
}
