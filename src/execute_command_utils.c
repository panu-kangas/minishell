#include "minishell.h"

void	free_all_from_process(char *cmd_path, char **e_args, char **env_var)
{
	free(cmd_path);
	ft_free_doubleptr(e_args);
	ft_free_doubleptr(env_var);
}

int	check_empty_input(char *cmd)
{
	if (cmd[0] == '\0')
		return (write_error(NULL, "\0", "command not found"));
	else
		return (0);
}

int	check_cmd_path(char *cmd)
{
	struct 	stat statbuf;

	if (access(cmd, F_OK == -1))
	{
		write_error(NULL, cmd, "No such file or directory");
		return (127);
	}
	if (stat(cmd, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 1)
		{
			write_error(NULL, cmd, "is a directory");
			return (126);
		}
	}
	if (access(cmd, X_OK) == -1)
	{
		write_error(NULL, cmd, "Permission denied");
		return (126);
	}
	return (0);
}