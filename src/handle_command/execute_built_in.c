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
	return (exit_status);
}