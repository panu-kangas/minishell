#include "minishell.h"

int	is_arg_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (i == 0 && arg[i] == '-')
			i++;
		if (ft_isdigit(arg[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_env_lst *env_lst, t_data *data, char **args)
{
	int	count;
	int	exit_status;

	ft_putendl_fd("exit", 2); // this doesn't appear if exit is piped, example: echo hello | exit --> WEIRD
	if (args[0] == NULL)
	{
		free_env_lst(env_lst);
		ft_free_data(data, 0);
		exit(0);
	}
	else if (is_arg_numeric(args[0]) == 0)
	{
		write_error("exit", args[0], "numeric argument required");
		free_env_lst(env_lst);
		ft_free_data(data, 0);
		exit(255);
	}
	count = 0;
	while (args[count] != NULL)
		count++;
	if (count > 1)
		return (write_error(NULL, "exit", "too many arguments"));
	exit_status = ft_atoi(args[0]) % 256;
	free_env_lst(env_lst);
	ft_free_data(data, 0);
	exit(exit_status);
}

int	execute_built_in(t_env_lst *env_lst, t_data *data, char *cmd, char **args)
{
	int	exit_status;

	exit_status = 0;

	if (is_echo(cmd) == 1)
	{
		if (ft_strncmp(args[0], "-n",  3) == 0)
			ft_echo(1, args);
		else
			ft_echo(0, args);
	}
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
	{
		if (args == NULL || *args == NULL)
			exit_status = ft_empty_export(env_lst);
		else
			exit_status = ft_export(env_lst, args[0]); // this needs to be a while loop to get all the args into ft_export.
	}
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_cd(env_lst, args[0]);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		ft_unset(env_lst, args[0]); // this needs to be a while loop to get all the args into ft_unset.
	else if (is_pwd(cmd) == 1)
		exit_status = ft_pwd();
	else if (is_env(cmd) == 1)
		ft_env(env_lst);
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_exit(env_lst, data, args);
	
	return (exit_status);
}