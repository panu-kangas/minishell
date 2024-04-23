#include "minishell.h"

int	write_error(char *cmd, char *specifier, char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(err_str, 2);
	return (ERR_STAT);
}

int	write_sys_error(char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(err_str);
	return (ERR_STAT); // this is probably a bit different depending on failure? Needs research
}