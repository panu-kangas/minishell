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
	return (1);
}

int	write_export_error(char *cmd, char *specifier, char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd("`", 2);
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd("\'", 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(err_str, 2);
	return (1);
}

int	write_sys_error(char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(err_str);
	return (1);
}

int	write_syntax_error(char *err_str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(err_str, 2);
	ft_putendl_fd("\'", 2);
	return (258);
}

int	write_weird_cd_error(char *err_str)
{
	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putendl_fd(err_str, 2);
	return (0);
}

int	write_amb_error(char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(err_str, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (ERR_STAT); // this is probably a bit different depending on failure? Needs research
}