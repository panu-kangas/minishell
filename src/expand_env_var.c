/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:37:32 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 13:10:23 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(t_env *env_lst, char *var_name)
{
	t_env	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
