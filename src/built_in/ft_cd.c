#include "minishell.h"

int	change_oldpwd_env_var(t_env_lst *env_lst, char *cur_dir) // MISTAKE!!! Use PWD value
{
	char		*env_name;

	env_name = ft_strjoin("OLDPWD=", cur_dir);
	free(cur_dir);
	if (env_name == NULL)
		return (write_sys_error("malloc failed")); // what err message for malloc...?
	if (process_non_global_env_node(env_lst, env_name) == ERR_STAT)
	{
		free(env_name);
		return (ERR_STAT);
	}
	free(env_name);
	return (0);
}

int	check_valid_path(char *path)
{
	struct stat statbuf;

	if (path == NULL)
		return (0);
	if (path[0] == '\0') // This is not error !!
		return (ERR_STAT); 
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

int	change_dir_to_home(t_env_lst *home, char *cur_dir)
{
	int	flag;

	if (check_valid_path(home->value) == ERR_STAT)
	{
		free(cur_dir);
		return (ERR_STAT);
	}
	else
		flag = chdir(home->value);
	return (flag);
}

int	ft_cd(t_env_lst *env_lst, char *path)
{
	t_env_lst	*home;
	int			flag;
	char		*cur_dir;

	if (check_valid_path(path) == ERR_STAT)
		return (ERR_STAT);
	home = check_if_var_exist(env_lst, "HOME");
	if (path == NULL && home == NULL)
		return (0);
	flag = 0;
	cur_dir = getcwd(NULL, 0); // Not needed !! Because of PWD
	if (path == NULL)
		flag = change_dir_to_home(home, cur_dir);
	else
		flag = chdir(path);
	if (flag < 0)
	{
		free(cur_dir);
		return (ERR_STAT);
	}
	change_oldpwd_env_var(env_lst, cur_dir); // MISTAKE!!! Use PWD value
	return (0);
}
