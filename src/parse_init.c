/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:01:29 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 11:57:48 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_parsing_cur_dir(char *parsing_cur_dir)
{
	char	*temp;
	int		i;
	int		j;

	temp = getcwd(NULL, 0);
	if (temp == NULL)
	{
		parsing_cur_dir[0] = '\0';
		return ;
	}
	i = 0;
	j = 0;
	while (temp[i] != '\0')
		parsing_cur_dir[j++] = temp[i++];
	parsing_cur_dir[j] = '\0';
	free(temp);
}


void	init_data(t_data *data, int exit_status, char *parsing_cur_dir)
{
	data->input = NULL;
	data->tokens = NULL;
	data->proc_count = 0;
	data->prev_exit_status = exit_status;
	copy_cur_dir_to_data(data, parsing_cur_dir); // diff init on first time ! maybe broken
}

void	init_file(t_file *new)
{
	new->filename = NULL;
	new->is_append = 0;
	new->is_infile = 0;
	new->no_filename = 0;
	new->hd_pipe[0] = -2;
	new->hd_pipe[1] = -2;
	new->quoted_heredoc = 0;
	new->next = NULL;
}

void	init_token(t_token *new)
{
	new->com = NULL;
	new->args = NULL;
	new->files = NULL;
	new->next = NULL;
	new->filecount = 0;
	new->comcount = 0;
	new->arg_count = 0;
}

void	init_parse(t_parse *new)
{
	new->str = NULL;
	new->isexpand = 0;
	new->isexpand = 0;
	new->next = NULL;
}
