#include "minishell.h"

int	update_pwd_env_var(t_data *data, t_env_lst *env_lst, char *cur_dir)
{
	t_env_lst	*pwd;
	t_env_lst	*old_pwd;
	char		*env_var;

	if (cur_dir == NULL)
		cur_dir = data->current_directory;
	pwd = check_if_var_exist(env_lst, "PWD");
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

	env_var = ft_strjoin("PWD=", cur_dir);
	if (env_var == NULL)
		return (write_sys_error("malloc failed"));
	if (process_non_global_env_node(env_lst, env_var) == 1)
	{
		free(env_var);
		return (1);
	}
	free(env_var);
	return (0);
}

int	change_dir_to_home(t_env_lst *home, t_data *data)
{
	int	flag;

	flag = analyze_path(home->value, data);
	if (flag != -1)
		return (flag);
	else
		flag = chdir(home->value);
	return (flag);
}

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
	while (i != 0)
	{
		if (path[i] == '/') // is there a problem with root folder (name just /)
		{
			if (path[i + 1] != '\0' && path[i + 1] != '/')
				break ;
		}
		i--;
	}
	if (i != 0)
		path[i] = '\0';
	return (0);
}

int	ft_cd(t_data *data, t_env_lst *env_lst, t_token *token)
{
	t_env_lst	*home;
	int			flag;
	char		*path;
	char		*cur_dir;

	path = token->args[0];
	if (path != NULL && path[0] == '\0')
		return (0);
	if (ft_strlen(path) >= 256)
		return (write_error("cd", path, "File name too long"));

	flag = analyze_path(path, data);
	if (flag >= 0)
		return (flag);
	else if (flag == -2)
	{
		if (get_absolute_parent_path(data, token) == -1)
			return (write_sys_error("malloc failed"));
		path = token->args[0];
	}
	
	home = check_if_var_exist(env_lst, "HOME");
	if (path == NULL && home == NULL)
		return (write_error(NULL, "cd", "HOME not set"));
	flag = 0;
	if (path == NULL)
	{
		flag = change_dir_to_home(home, data);
		if (flag != -1)
			return (flag);
	}
	else
		flag = chdir(path);
	if (flag < 0)
		return (write_error("cd", path, "Permission denied"));
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		write_sys_error("cd: error retrieving current directory: getcwd: cannot access parent directories");
		add_path_to_data_cur_dir(data, path);
	}
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
