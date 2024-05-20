#include "minishell.h"

void	alter_termios(int flag)
{
	struct termios	term;

	tcgetattr(2, &term); // error handling ??

	if (flag == 0)
		term.c_lflag &= ~ECHOCTL;
	else
		term.c_lflag |= ECHOCTL;

	tcsetattr(0, TCSANOW, &term);  // error handling ??
}

void	sig_handler_main(int signum)
{
	if (signum == 2)
	{
		ft_putendl_fd("", 1);
		rl_on_new_line(); // do we have to check for return value ??
		rl_replace_line("", 0); // do we have to check for return value ??
		rl_redisplay(); // do we have to check for return value ??
		g_signal_marker = 2;
	}
}

void	process_signal_main(void)
{
	struct sigaction sigact;

	sigact.sa_handler = &sig_handler_main;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sigact, NULL); // do we have to check for return value ??
	signal(SIGQUIT, SIG_IGN); // change to SIGACTION
}
