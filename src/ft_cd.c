#include "minishell.h"

int	update_pwd_env_var(t_env_lst *env_lst, char *cur_dir)
{
	t_env_lst	*pwd;
	t_env_lst	*old_pwd;
	char		*env_var;

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

int	ft_cd(t_data *data, t_env_lst *env_lst, char *path)
{
	t_env_lst	*home;
	int			flag;
	char		*cur_dir;

	if (path != NULL && path[0] == '\0')
		return (0);
	if (ft_strlen(path) >= 256)
		return (write_error("cd", path, "File name too long"));
	flag = analyze_path(path, data);
	if (flag != -1)
		return (flag);
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
		return (1);
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
		return (write_sys_error("malloc failed"));
	copy_cur_dir_to_data(data, cur_dir);
	if (update_pwd_env_var(env_lst, cur_dir) == 1)
	{
		free(cur_dir);
		return (1);
	}
	free(cur_dir);
	return (0);
}
