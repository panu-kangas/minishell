#include "minishell.h"

int	execute_built_in(t_env_lst *env_lst, char *cmd, char **args)
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

	// is exit needed here...? Or checked at parsing already?
	
	return (exit_status);
}