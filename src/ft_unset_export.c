/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:32:29 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/06 14:17:57 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	move_env_lst(t_env *env_lst)
{
	t_env	*next_node;

	next_node = env_lst->next;
	free(env_lst->name);
	free(env_lst->value);
	while (next_node != NULL)
	{
		env_lst->name = next_node->name;
		env_lst->value = next_node->value;
		env_lst->is_global = next_node->is_global;
		if (next_node->next == NULL)
			env_lst->next = NULL;
		env_lst = next_node;
		next_node = next_node->next;
	}
	free(env_lst);
	return (0);
}

int	ft_unset(t_env *env_lst, char *var_name)
{
	t_env	*temp;
	t_env	*temp_prev;

	if (unset_check_valid_input(var_name) == 1)
		return (1);
	if (ft_strncmp(var_name, "_", 2) == 0)
		return (0);
	temp = env_lst;
	temp_prev = env_lst;
	if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
		return (move_env_lst(env_lst));
	while (temp->next != NULL)
	{
		temp_prev = temp;
		temp = temp->next;
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
		{
			temp_prev->next = temp->next;
			delete_env_node(temp);
			break ;
		}
	}
	return (0);
}

t_env	*check_if_var_exist(t_env *env_lst, char *var_name)
{
	while (env_lst != NULL)
	{
		if (ft_strncmp(env_lst->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (env_lst);
		env_lst = env_lst->next;
	}
	return (NULL);
}

int	execute_export(t_env *env_lst, t_env *temp, char *new_env_var)
{
	int	flag;

	flag = 0;
	if (temp == NULL || temp->is_global == 1)
	{
		if (ft_strchr(new_env_var, '=') == NULL)
			flag = process_null_value_env_node(env_lst, new_env_var);
		else
			flag = process_global_env_node(env_lst, new_env_var);
	}
	else
	{
		if (ft_strchr(new_env_var, '=') == NULL)
			temp->is_global = 1;
		else
		{
			flag = process_global_env_node(env_lst, new_env_var);
			if (flag != 1)
				temp->is_global = 1;
		}
	}
	return (flag);
}

int	ft_export(t_env *env_lst, char *new_env_var)
{
	int		flag;
	char	*var_name;
	t_env	*temp;

	if (export_check_valid_input(new_env_var) == 1)
		return (1);
	var_name = get_var_name(new_env_var);
	if (var_name == NULL)
		return (1);
	temp = check_if_var_exist(env_lst, var_name);
	free(var_name);
	flag = execute_export(env_lst, temp, new_env_var);
	if (flag == 1)
		return (1);
	else
		return (0);
}
