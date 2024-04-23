/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:01:29 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/23 15:15:53 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_data(t_data *data)
{
	data->input = NULL;
	data->tokens = NULL;
	data->pipecount = 0;
}

void	init_file(t_file *new)
{
	new->filename = NULL;
	new->is_append = 0;
	new->is_infile = 0;
	new->no_filename = 0;
	new->next = NULL;
}

void	init_token(t_token *new)
{
	new->com = NULL;
	new->args = NULL;
	new->files = NULL;
	new->next = NULL;
	new->filecount = 0;
}
