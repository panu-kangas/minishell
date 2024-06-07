/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:19:26 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 14:08:27 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_infile(t_data *data, t_token *cur_token)
{
	t_file	*file;
	int		exit_status;

	file = cur_token->files;
	exit_status = 0;
	while (file != NULL)
	{
		if (file->is_infile == 1)
		{
			if (file->is_append == -1)
				exit_status = open_infile(data, file->filename, file);
			else if (file->is_append == 1)
			{
				if (dup2(file->hd_pipe[0], 0) == -1)
					return (write_sys_error("dup2 failed"));
			}
			if (exit_status != 0)
				break ;
		}
		file = file->next;
	}
	return (exit_status);
}

int	create_extra_outfiles(t_data *data, t_token *cur_token, int ofile_count)
{
	t_file	*file;
	t_file	*temp;

	file = NULL;
	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
		{
			if (file != NULL && ofile_count > 1)
			{
				if (ft_just_create_file(data, file->filename, file) == 1)
					return (1);
				ofile_count--;
			}
			file = temp;
		}
		temp = temp->next;
	}
	return (0);
}

int	handle_infile(t_data *data, t_token *cur_token, int index, int **fd_pipes)
{
	int	exit_status;

	if (index != 0 && check_for_infile(cur_token) == 0)
		exit_status = dup_pipe_in(fd_pipes, index);
	else
		exit_status = process_infile(data, cur_token);
	if (exit_status != 0)
	{
		close_all_pipes(data, fd_pipes, (data->proc_count - 1));
		ft_free_int_doubleptr(fd_pipes);
		return (exit_status);
	}
	return (0);
}

int	handle_outfile(t_data *data, t_token *cur_token, int index, int **fd_pipes)
{
	int		ofile_count;
	int		exit_status;
	t_file	*file;

	exit_status = 0;
	ofile_count = count_outfiles(cur_token);
	if (ofile_count > 1)
	{
		exit_status = create_extra_outfiles(data, cur_token, ofile_count);
		if (exit_status != 0)
			return (exit_status);
	}
	file = find_outfile(cur_token);
	if (index != (data->proc_count - 1) && check_for_outfile(cur_token) == 0)
		exit_status = dup_pipe_out(fd_pipes, index);
	else if (file != NULL)
	{
		if (file->is_append == -1)
			exit_status = open_outfile(data, file->filename, file);
		else if (file->is_append == 1)
			exit_status = open_outfile_append(data, file->filename, file);
	}
	return (exit_status);
}

int	ft_redirect(t_data *data, int **fd_pipes, int index)
{
	int		exit_status;
	t_token	*cur_token;

	cur_token = get_cur_token(data, index);
	exit_status = handle_infile(data, cur_token, index, fd_pipes);
	if (exit_status != 0)
		return (exit_status);
	exit_status = handle_outfile(data, cur_token, index, fd_pipes);
	close_all_pipes(data, fd_pipes, (data->proc_count - 1));
	ft_free_int_doubleptr(fd_pipes);
	return (exit_status);
}
