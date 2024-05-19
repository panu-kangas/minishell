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

int	check_valid_path(char *path)
{
	struct stat statbuf;

	if (path == NULL)
		return (0);
	if (access(path, F_OK) == -1)
		return (write_error("cd", path, "No such file or directory"));
	if (stat(path, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 0)
			return (write_error("cd", path, "Not a directory"));
	}
	if (access(path, X_OK) == -1)
		return (write_error("cd", path, "Permission denied"));
	return (0);
}

int	change_dir_to_home(t_env_lst *home)
{
	int	flag;

	if (check_valid_path(home->value) == ERR_STAT)
		return (ERR_STAT);
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
	if (check_valid_path(path) == ERR_STAT)
		return (ERR_STAT);
	home = check_if_var_exist(env_lst, "HOME");
	if (path == NULL && home == NULL)
		return (write_error(NULL, "cd", "HOME not set"));
	flag = 0;
	if (path == NULL)
		flag = change_dir_to_home(home);
	else
		flag = chdir(path);
	if (flag < 0)
		return (ERR_STAT);
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
