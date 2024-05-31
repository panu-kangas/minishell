/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:37:32 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 10:37:33 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(t_env_lst *env_lst, char *var_name)
{
	t_env_lst	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char	**expand_env_var_array(t_env_lst *env_lst, char *var_name)
{
	t_env_lst	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (ft_split(temp->value, ' '));
		temp = temp->next;
	}
	return (NULL);
}
