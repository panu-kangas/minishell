/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:14:13 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/07 11:23:11 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_error(char *cmd, char *specifier, char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(err_str, 2);
	return (1);
}

int	write_exp_error(char *cmd, char *specifier)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd("`", 2);
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd("\'", 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("not a valid identifier", 2);
	return (1);
}

int	write_sys_error(char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(err_str);
	return (1);
}

int	write_syntax_error(char *err_str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(err_str, 2);
	ft_putendl_fd("\'", 2);
	return (258);
}

int	write_amb_error(char *err_str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(err_str, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (1);
}
