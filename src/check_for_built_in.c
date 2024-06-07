/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_built_in.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:18:09 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/30 15:18:12 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env(char *cmd)
{
	if (ft_strlen(cmd) != 3)
		return (0);
	if (cmd[0] != 'e' && cmd[0] != 'E')
		return (0);
	if (cmd[1] != 'n' && cmd[1] != 'N')
		return (0);
	if (cmd[2] != 'v' && cmd[2] != 'V')
		return (0);
	return (1);
}

int	is_pwd(char *cmd)
{
	if (ft_strlen(cmd) != 3)
		return (0);
	if (cmd[0] != 'p' && cmd[0] != 'P')
		return (0);
	if (cmd[1] != 'w' && cmd[1] != 'W')
		return (0);
	if (cmd[2] != 'd' && cmd[2] != 'D')
		return (0);
	return (1);
}

int	is_echo(char *cmd)
{
	if (ft_strlen(cmd) != 4)
		return (0);
	if (cmd[0] != 'e' && cmd[0] != 'E')
		return (0);
	if (cmd[1] != 'c' && cmd[1] != 'C')
		return (0);
	if (cmd[2] != 'h' && cmd[2] != 'H')
		return (0);
	if (cmd[3] != 'o' && cmd[3] != 'O')
		return (0);
	return (1);
}

int	check_for_built_in(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (is_echo(cmd) == 1)
		return (1);
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		return (1);
	else if (is_pwd(cmd) == 1)
		return (1);
	else if (is_env(cmd) == 1)
		return (1);
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		return (1);
	return (0);
}
