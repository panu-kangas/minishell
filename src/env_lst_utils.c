/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:18:30 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/30 15:18:32 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_env_node(t_env_lst *ptr)
{
	free(ptr->name);
	free(ptr->value);
	free(ptr);
}

void	free_env_lst(t_env_lst *env_lst)
{
	t_env_lst	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		temp = env_lst->next;
		delete_env_node(env_lst);
		env_lst = temp;
	}
}
