#include "minishell.h"

// This doesn't work =(

/* void	disable_termios_echo(void)
{
//	int				tty_fd;
//	char			*tty_name;
	struct termios	term;

	if (isatty(2) == 1) // is this ok? Not sure yet....
		tty_name = ttyname(2); // error handling ??

	tty_fd = open(tty_name, O_RDONLY); // remember to close the FD!! AND check for -1 return value. Also: is the flag O_RDONLY ok?
	
	tcgetattr(0, &term); // error handling ??

	term.c_lflag |= ECHO;
	term.c_lflag |= ECHOCTL;

	tcsetattr(0, TCSANOW, &term);  // error handling ??
	// CLOSE THE TTY FD
}
*/


void	sig_handler_main(int signum)
{
	if (signum == 2)
	{
		rl_redisplay(); // do we have to check for return value ??

		write(1, "  ", 2); // THIS NEEDS TO BE CHECKED!! Probably not right.

		ft_putendl_fd("", 1);
		rl_on_new_line(); // do we have to check for return value ??
		rl_replace_line("", 0); // do we have to check for return value ??
		rl_redisplay(); // do we have to check for return value ??
	}
}

void	process_signal_main(void)
{
	struct sigaction sigact;

//	disable_termios_echo();

	sigact.sa_handler = &sig_handler_main;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sigact, NULL); // do we have to check for return value ??
	signal(SIGQUIT, SIG_IGN); // change to SIGACTION
}
