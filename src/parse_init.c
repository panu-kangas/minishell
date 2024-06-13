/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:01:29 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/13 09:56:14 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_parsing_cur_dir(char *parsing_cur_dir)
{
	char	*temp;
	int		i;
	int		j;

	temp = getcwd(NULL, 0);
	if (temp == NULL)
		return (1);
	i = 0;
	j = 0;
	while (temp[i] != '\0')
		parsing_cur_dir[j++] = temp[i++];
	parsing_cur_dir[j] = '\0';
	free(temp);
	return (0);
}

void	init_data(t_data *data, int exit_status, char *parsing_cur_dir)
{
	data->input = NULL;
	data->tokens = NULL;
	data->proc_count = 0;
	data->prev_exit_status = exit_status;
	copy_cur_dir_to_data(data, parsing_cur_dir);
}

void	init_file(t_file *new)
{
	new->filename = NULL;
	new->is_append = 0;
	new->is_infile = 0;
	new->is_amb = 0;
	new->no_filename = 0;
	new->hd_pipe[0] = -2;
	new->hd_pipe[1] = -2;
	new->quoted_heredoc = 0;
	new->next = NULL;
}

void	init_token(t_token *new)
{
	new->com = NULL;
	new->com_is_expanded_empty = 0;
	new->arg_is_expanded_empty = -1;
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
