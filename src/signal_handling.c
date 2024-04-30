#include "minishell.h"

void	sig_handler(int signum)
{
	ft_printf("test\n");
	return ;
}

void	signal_handling(void)
{
	struct sigaction sigact;

	ft_bzero(&sigact, sizeof(sigact)); // is this needed?
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	sigaction(SIGQUIT, &sigact, NULL);
	sigaction(SIG) // how to catch ctrl-D ??
}