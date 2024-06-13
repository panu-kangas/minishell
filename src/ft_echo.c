/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:34:38 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/13 10:40:12 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(int flag, char **args, t_token *t)
{
	int	i;

	i = 0;
	while (check_echo_flag(args[i]) == 1)
		i++;
	while (args != NULL && args[i] != NULL)
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i] != NULL && t->arg_is_expanded_empty != i - 1)
			ft_putchar_fd(' ', 1);
	}
	if (flag == 0)
		ft_putendl_fd("", 1);
}
