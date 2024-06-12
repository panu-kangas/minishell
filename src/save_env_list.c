/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:52:58 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/12 14:49:53 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_shlvl(t_env *shlvl)
{
	int	value;

	if (shlvl->value == NULL || shlvl->value[0] == '\0')
		value = 0;
	else
		value = ft_atoi(shlvl->value);
	value++;
	if (value < 0)
		value = 0;
	else if (value > 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		ft_putnbr_fd(value, 2);
		ft_putendl_fd(") too high, resetting to 1", 2);
		value = 1;
	}
	free(shlvl->value);
	if (value == 1000)
		shlvl->value = ft_strdup("");
	else
		shlvl->value = ft_itoa(value);
	if (shlvl->value == NULL)
		return (1);
	else
		return (0);
}

int	process_shlvl(t_env *env_lst)
{
	t_env	*shlvl;

	shlvl = check_if_var_exist(env_lst, "SHLVL");
	if (shlvl == NULL)
	{
		shlvl = get_global_env_node("SHLVL=1");
		if (shlvl == NULL)
			return (1);
		shlvl->next = env_lst->next;
		env_lst->next = shlvl;
	}
	else
		return (change_shlvl(shlvl));
	return (0);
}

int	create_pwd(t_env *temp)
{
	char		*cur_dir;
	char		*pwd;

	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
		return (1);
	pwd = ft_strjoin("PWD=", cur_dir);
	free(cur_dir);
	if (pwd == NULL)
		return (1);
	temp->next = get_global_env_node(pwd);
	free(pwd);
	if (temp->next == NULL)
		return (1);
	temp = temp->next;
	temp->next = get_non_global_env_node("1secret_env_var=amazing");
	if (temp->next == NULL)
		return (1);
	return (0);
}

int	make_pwd_variables(t_env *env_lst)
{
	t_env	*temp;

	temp = env_lst;
	while (temp->next != NULL)
		temp = temp->next;
	if (check_if_var_exist(env_lst, "OLDPWD") == NULL)
	{
		temp->next = get_null_value_env_node("OLDPWD");
		if (temp->next == NULL)
			return (1);
		temp = temp->next;
	}
	if (check_if_var_exist(env_lst, "PWD") == NULL)
		return (create_pwd(temp));
	return (0);
}

t_env	*save_env_list(char **environ)
{
	int		i;
	t_env	*env_lst;
	t_env	*temp;

	env_lst = NULL;
	if (environ != NULL && environ[0] != NULL)
		env_lst = get_global_env_node(environ[0]);
	else
		env_lst = get_null_value_env_node("OLDPWD");
	if (env_lst == NULL)
		return (NULL);
	temp = env_lst;
	i = 1;
	while (environ != NULL && environ[0] != NULL && environ[i] != NULL)
	{
		temp->next = get_global_env_node(environ[i++]);
		temp = temp->next;
		if (temp == NULL)
			return (NULL);
	}
	if (make_pwd_variables(env_lst) == 1)
		return (NULL);
	if (process_shlvl(env_lst) == 1)
		return (NULL);
	return (env_lst);
}
