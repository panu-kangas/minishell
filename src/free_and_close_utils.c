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
