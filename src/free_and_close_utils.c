/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_close_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:58:06 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 12:58:09 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_hd_str(char *str, int exit_status)
{
	free(str);
	return (exit_status);
}

void	close_std_fd(int *std_fd)
{
	close_and_init_fd(&std_fd[0]);
	close_and_init_fd(&std_fd[1]);
}

int	check_for_hd_error(int i, int prev_i, int *whole_len)
{
	if (prev_i == i)
		return (write_sys_error("malloc failed"));
	*whole_len += 1;
	if (*whole_len > 60000)
		return (write_error(NULL, "heredoc error", "heredoc input too long"));
	return (0);
}
