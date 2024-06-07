/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:35:21 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 13:12:24 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_arg_numeric(char *arg)
{
	int	i;
	int	len;

	i = 0;
	while (arg[i] != '\0')
	{
		if (i == 0 && (arg[i] == '-' || arg[i] == '+'))
			i++;
		if (ft_isdigit(arg[i]) == 0)
			return (0);
		i++;
	}
	len = ft_strlen(arg);
	if ((arg[0] == '-' || arg[0] == '+') && len > 20)
		return (0);
	else if ((arg[0] != '-' && arg[0] != '+') && len > 19)
		return (0);
	if (arg[0] == '-' && len == 20 && ft_atoi(arg) == 0 && arg[19] != '8')
		return (0);
	else if (arg[0] == '+' && len == 20 && ft_atoi(arg) == -1 && arg[19] != '7')
		return (0);
	else if (len == 19 && ft_atoi(arg) == -1 && arg[18] != '7')
		return (0);
	return (1);
}

void	free_all_exit(t_data *data, t_env *env_lst)
{
	free_env_lst(env_lst);
	ft_free_data(data, 0);
}

int	ft_exit(t_env *env_lst, t_data *data, char **args, int e_flag)
{
	int	count;
	int	exit_status;

	if (e_flag == 1)
		ft_putendl_fd("exit", 2);
	if (args[0] == NULL)
	{
		exit_status = data->prev_exit_status;
		free_all_exit(data, env_lst);
		exit(exit_status);
	}
	else if (is_arg_numeric(args[0]) == 0)
	{
		write_error("exit", args[0], "numeric argument required");
		free_all_exit(data, env_lst);
		exit(255);
	}
	count = 0;
	while (args[count] != NULL)
		count++;
	if (count > 1)
		return (write_error(NULL, "exit", "too many arguments"));
	exit_status = ft_atoi(args[0]) % 256;
	free_all_exit(data, env_lst);
	exit(exit_status);
}
