#include "minishell.h"

// Give NULL, if no args are given.

void	ft_echo(int flag, char **args)
{
	int	i;

	i = 0;
	if (flag == 1)
		i++;
	while (args[i] != NULL)
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (flag == 0)
		ft_putendl_fd("", 1);
}
