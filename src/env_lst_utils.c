/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:20:25 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 13:20:27 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_env_node(t_env *ptr)
{
	free(ptr->name);
	free(ptr->value);
	free(ptr);
}

void	free_env_lst(t_env *env_lst)
{
	t_env	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		temp = env_lst->next;
		delete_env_node(env_lst);
		env_lst = temp;
	}
}

void	free_all_after_execve_error(char *cmd_path, char **env_var_arr)
{
	free(cmd_path);
	ft_free_doubleptr(env_var_arr);
	write_sys_error("execve failed");
}

void	free_and_exit(t_data *data, t_env *env_lst)
{
	int	exit_status;

	exit_status = data->prev_exit_status;
	alter_termios(1);
	free(data);
	free_env_lst(env_lst);
	ft_putstr_fd("exit\n", 2);
	exit(exit_status);
}

int	check_is_cmd_path(char *cmd)
{
	if (cmd != NULL && ft_strchr(cmd, '/') != NULL)
		return (1);
	else if (ft_strncmp(cmd, ".", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd, "..", 3) == 0)
		return (1);
	else
		return (0);
}
