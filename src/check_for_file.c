/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:10:47 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 16:10:54 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_outfile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	check_for_infile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == 1)
			return (1);
		temp = temp->next;
	}
	return (0);
}
