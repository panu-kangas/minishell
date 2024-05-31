/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:18:45 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/30 15:18:47 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_export(t_env_lst *env_lst, char **args)
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

int	handle_unset(t_env_lst *env_lst, char **args)
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

void	handle_echo(char **args)
{
	if (ft_strncmp(args[0], "-n", 3) == 0)
		ft_echo(1, args);
	else
		ft_echo(0, args);
}

int	execute_built_in(t_env_lst *env_lst, t_data *data, t_token *token)
{
	int		exit_status;
	char	*cmd;
	char	**args;

	exit_status = 0;
	cmd = token->com;
	args = token->args;
	if (is_echo(cmd) == 1)
		handle_echo(args);
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		exit_status = handle_export(env_lst, args);
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_cd(data, env_lst, token);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		exit_status = handle_unset(env_lst, args);
	else if (is_pwd(cmd) == 1)
		exit_status = ft_pwd(data);
	else if (is_env(cmd) == 1)
		ft_env(env_lst);
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		exit_status = ft_exit(env_lst, data, args);
	return (exit_status);
}
