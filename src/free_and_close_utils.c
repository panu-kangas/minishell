#include "minishell.h"

int	free_hd_str(char *str)
{
	free(str);
	return (1);
}

void	close_std_fd(int *std_fd)
{
	close_and_init_fd(&std_fd[0]);
	close_and_init_fd(&std_fd[1]);
}
