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
		return (write_sys_error("termios alteration failed"));
	if (flag == 0)
		term.c_lflag &= ~ECHOCTL;
	else
		term.c_lflag |= ECHOCTL;
	if (tcsetattr(2, TCSANOW, &term) == -1)
		return (write_sys_error("termios alteration failed"));
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

void	set_signals_to_dfl_or_ign(int flag)
{
	if (flag == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	process_signal_main(void)
{
	signal(SIGINT, &sig_handler_main);
	signal(SIGQUIT, SIG_IGN);
}
