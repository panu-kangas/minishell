/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 13:47:54 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 15:19:02 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alter_termios(int flag)
{
	struct termios	term;

	if (tcgetattr(2, &term) == -1)
		return (write_sys_error("termios alteration failed")); // How to handle this in main ??
	if (flag == 0)
		term.c_lflag &= ~ECHOCTL;
	else
		term.c_lflag |= ECHOCTL;
	if (tcsetattr(2, TCSANOW, &term) == -1)
		return (write_sys_error("termios alteration failed")); // How to handle this in main ??
	return (0);
}

void	sig_handler_hd(int signum)
{
	if (signum == 2)
	{
		ft_putendl_fd("", 1);
		close(STDIN_FILENO);
		g_signal_marker = 2;
	}
}

void	sig_handler_main(int signum)
{
	if (signum == 2)
	{
		ft_putendl_fd("", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_marker = 2;
	}
}

void	set_signals_to_dfl(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	process_signal_main(void)
{
	struct sigaction	sigact;

	sigact.sa_handler = &sig_handler_main;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sigact, NULL) == -1)
		return (write_sys_error("sigaction failed")); // How to handle this in main ??
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
