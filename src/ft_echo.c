/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:34:38 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/12 15:08:24 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(int flag, char **args)
{
	int	i;

	i = 0;
	while (check_echo_flag(args[i]) == 1)
		i++;
	while (args != NULL && args[i] != NULL)
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (flag == 0)
		ft_putendl_fd("", 1);
}
