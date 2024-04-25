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

int	handle_command(t_data *data, t_env_lst *env_lst, int index)
{
	int		exit_status;
	char	**execve_args;
	t_token	*cur_token;

	cur_token = get_cur_token(data, index);

	if (check_for_built_in(cur_token->com) == 1)
		exit_status = execute_built_in(env_lst, cur_token->com, cur_token->args);
	else
	{
		execve_args = get_execve_args(cur_token->com, cur_token->args);
		if (execve_args == NULL)
			return (write_sys_error("malloc failed"));
		exit_status = execute_command(cur_token->com, execve_args, env_lst, data);
		ft_free_doubleptr(execve_args);
	}
	return (exit_status);
}
