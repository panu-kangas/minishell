/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:33:05 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 13:13:19 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_global_env_node(char *environ_var)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = get_var_value(environ_var);
	if (new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->is_global = 1;
	return (new_node);
}

t_env	*get_non_global_env_node(char *environ_var)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = get_var_value(environ_var);
	if (new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->is_global = 0;
	return (new_node);
}

t_env	*get_null_value_env_node(char *environ_var)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = NULL;
	new_node->next = NULL;
	new_node->is_global = 1;
	return (new_node);
}
