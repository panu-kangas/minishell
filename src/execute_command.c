#include "minishell.h"

int	is_a_path(char *cmd, int *exit_status)
{
	char *cmd_path;

	if (cmd != NULL && ft_strchr(cmd, '/') != NULL)
	{
		*exit_status = check_cmd_path(cmd);
		if (*exit_status != 0)
			return (NULL);
		cmd_path = ft_strdup(cmd);
		return (cmd_path);
	}
}

char	*find_cmd_path(char *cmd, char **paths, int *exit_status)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (cmd != NULL && ft_strchr(cmd, '/') != NULL)
		return (is_a_path(cmd, &exit_status));
	cmd_path = NULL;
	while (paths != NULL && paths[i] != NULL)
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
		cmd_path = NULL;
		i++;
	}
	if (paths != NULL && paths[i] == NULL)
	{
		write_error(NULL, cmd, "command not found");
		*exit_status = 127;
		return (NULL);
	}
	return (cmd_path);
}

// Should I free everything the process needs here? So on every error return, there maybe should be also a free_all -call.
// what if I had a "write_execve_error" -function: it could have a free_all -call included!

int	execute_command(char *cmd, char **e_args, t_env_lst *env_lst, t_data *data)
{
	char	*cmd_path;
	char	**paths;
	char	**env_var;
	int		exit_status;

	if (check_empty_input(cmd) == ERR_STAT)
		return (127);

	if (check_if_var_exist(env_lst, "PATH") != NULL)
	{
		paths = get_paths(env_lst);
		if (paths == NULL)
			return (write_sys_error("malloc failed"));
	}
	else
		paths = NULL;
	if (paths == NULL && ft_strchr(cmd, '/') == NULL)
		return (write_error(NULL, cmd, "No such file or directory"));

	cmd_path = find_cmd_path(cmd, paths, &exit_status);
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
	paths = NULL;

	env_var = make_env_var_array(env_lst);
	if (env_var == NULL)
	{
		ft_free_doubleptr(paths);
		free(cmd_path);
		return (write_sys_error("malloc failed"));
	}
	free_env_lst(env_lst);
	ft_free_data(data, 0);

	if (execve(cmd_path, e_args, env_var) == -1)
	{
		free_all_from_process(cmd_path, e_args, env_var);
		return (write_sys_error("execve failed"));
	}
	return (1);
}