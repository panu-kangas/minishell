/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:18:45 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/13 11:36:07 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_export(t_env *env_lst, char **args)
{
	int	i;
	int	temp;
	int	exit_status;

	exit_status = 0;
	temp = 0;
	if (args == NULL || *args == NULL)
		return (ft_empty_export(env_lst));
	i = 0;
	while (args[i] != NULL)
	{
		temp = ft_export(env_lst, args[i]);
		if (temp != 0)
			exit_status = temp;
		i++;
	}
	return (exit_status);
}

int	handle_unset(t_env *env_lst, char **args)
{
	int	i;
	int	temp;
	int	exit_status;

	exit_status = 0;
	temp = 0;
	if (args == NULL || args[0] == NULL)
		return (0);
	i = 0;
	while (args[i] != NULL)
	{
		temp = ft_unset(env_lst, args[i]);
		if (temp != 0)
			exit_status = temp;
		i++;
	}
	return (exit_status);
}

int	check_echo_flag(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-')
		return (0);
	i = 1;
	if (arg[i] == '\0')
		return (0);
	while (arg[i] != '\0')
	{
		if (arg[i] == 'n')
			i++;
		else
			return (0);
	}
	return (1);
}

void	handle_echo(char **args, t_token *t)
{
	if (check_echo_flag(args[0]) == 1)
		ft_echo(1, args, t);
	else
		ft_echo(0, args, t);
}

int	execute_built_in(t_env *env_lst, t_data *data, t_token *t, int e_flag)
{
	int		exit_status;
	char	*cmd;
	char	**args;

	exit_status = 0;
	cmd = t->com;
	args = t->args;
	if (is_echo(cmd) == 1)
		handle_echo(args, t);
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		exit_status = handle_export(env_lst, args);
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_cd(data, env_lst, t);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		exit_status = handle_unset(env_lst, args);
	else if (is_pwd(cmd) == 1)
		exit_status = ft_pwd(data);
	else if (is_env(cmd) == 1)
		ft_env(env_lst);
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_exit(env_lst, data, args, e_flag);
	return (exit_status);
}
