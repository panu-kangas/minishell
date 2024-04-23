#include "minishell.h"

int	get_args_size(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}	

char	**just_the_cmd_doubleptr(char *cmd)
{
	char	**dptr;

	dptr = malloc(2 * sizeof(char *));
	if (dptr == NULL)
		return (NULL);
	dptr[0] = ft_strdup(cmd);
	if (dptr[0] == NULL)
	{
		ft_free_doubleptr(dptr);
		return (NULL);
	}
	dptr[1] = NULL;
	return (dptr);
}

char	**get_execve_args(char *cmd, char **args)
{
	char	**new_args;
	char	*temp;
	int		i;

	if (args == NULL)
		return (just_the_cmd_doubleptr(cmd));
	i = 0;
	new_args = malloc((get_args_size(args) + 2) * sizeof(char *));
	if (new_args == NULL)
		return (NULL);
	new_args[0] = ft_strdup(cmd);
	if (new_args[0] == NULL)
	{
		ft_free_doubleptr(new_args);
		return (NULL);
	}
	i = 1;
	while (args[i - 1] != NULL)
	{
		new_args[i] = ft_strdup(args[i - 1]);
		if (new_args[i] == NULL)
		{
			ft_free_doubleptr(new_args);
			return (NULL);
		}
		i++;
	}
	new_args[i] = NULL;
	return (new_args);
}

int	handle_command(t_env_lst *env_lst, char *cmd, char **args)
{
	int		exit_status;
	char	**execve_args;

	if (check_for_built_in(cmd) == 1)
		exit_status = execute_built_in(env_lst, cmd, args);
	else
	{
		execve_args = get_execve_args(cmd, args);
		if (execve_args == NULL)
			return (write_sys_error("malloc failed"));
		exit_status = execute_command(cmd, execve_args, env_lst); // THIS NEEDS TO HAPPEN IN A CHILD PROCESS!!! So we need to fork before this. Otherwise execve will take over
		if (exit_status != 0)
			ft_free_doubleptr(execve_args);
	}
	return (exit_status);
}
