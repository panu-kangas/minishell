/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:17:17 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 16:17:18 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_cur_token(t_data *data, int index)
{
	t_token	*cur_token;

	cur_token = data->tokens;
	if (index == 0)
		return (cur_token);
	else
	{
		while (index > 0)
		{
			cur_token = cur_token->next;
			index--;
		}
		return (cur_token);
	}
}

int	count_outfiles(t_token *cur_token)
{
	int		count;
	t_file	*temp;

	count = 0;
	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			count++;
		temp = temp->next;
	}
	return (count);
}

t_file	*find_outfile(t_token *cur_token)
{
	t_file	*file;
	t_file	*temp;

	file = NULL;
	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			file = temp;
		temp = temp->next;
	}
	return (file);
}

int	dup_pipe_out(int **fd_pipes, int index)
{
	int	out_fd;

	out_fd = fd_pipes[index][1];
	if (dup2(out_fd, 1) == -1)
		return (write_sys_error("dup2 failed"));
	return (0);
}

int	dup_pipe_in(int **fd_pipes, int index)
{
	int	in_fd;

	in_fd = fd_pipes[index - 1][0];
	if (dup2(in_fd, 0) == -1)
		return (write_sys_error("dup2 failed"));
	return (0);
}
