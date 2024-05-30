/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 08:09:54 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 09:52:54 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_file_ms(t_file **files, t_file *new)
{
	t_file	*temp;

	temp = *files;
	if (*files == NULL)
		*files = new;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
		new->next = NULL;
	}
}

void	ft_lstadd_back_ms(t_token **tokens, t_token *newnode)
{
	t_token	*temp;

	temp = *tokens;
	if (*tokens == NULL)
		*tokens = newnode;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newnode;
		newnode->next = NULL;
	}
}
