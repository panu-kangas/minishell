#include "minishell.h"

void	sigquit_handler(int signum)
{
	ft_printf("test\n");
	return ;
}

void	signal_handling(void)
{
	struct sigaction sigact;

	ft_bzero(&sigact, sizeof(sigact));
	sigact.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &sigact, NULL);
}