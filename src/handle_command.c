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

int	change_command(t_token *cur_token)
{
	int	i;

	free(cur_token->com);
	cur_token->com = cur_token->args[0];
	if (cur_token->args[1] == NULL)
	{
		cur_token->args[0] = NULL;
		return (2);
	}
	i = 0;
	while (cur_token->args[i + 1] != NULL)
	{
		cur_token->args[i] = cur_token->args[i + 1];
		i++;
	}
	cur_token->args[i] = NULL;
	return (2);
}

int	check_for_env_var(t_token *cur_token, t_env_lst *env_lst) // IN PROCESS
{
	int	exit_status;

	exit_status = 0;
	if (cur_token->com[0] == '=')
		return (2);
	else if (cur_token->args != NULL && *cur_token->args != NULL)
		return (change_command(cur_token));
	else
		exit_status = process_non_global_env_node(env_lst, cur_token->com);
	return (exit_status);
}

int	handle_command(t_data *data, t_env_lst *env_lst, int index)
{
	int		exit_status;
	char	**execve_args;
	t_token	*cur_token;

	cur_token = get_cur_token(data, index);

	if (cur_token->com != NULL && ft_strchr(cur_token->com, '=') != NULL) // IN PROCESS
	{
		exit_status = check_for_env_var(cur_token, env_lst);
		if (exit_status != 2)
			return (exit_status);
	}

	if (check_for_built_in(cur_token->com) == 1)
		exit_status = execute_built_in(env_lst, data, cur_token);
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
