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

int	process_infile(t_token *cur_token)
{
	t_file	*file;
	int		exit_code;

	file = cur_token->files;
	exit_code = 0;

	while (file != NULL)
	{
		if (file->is_infile == 1)
		{
			if (file->is_append == -1)
				exit_code = open_infile(file->filename);
			else if (file->is_append == 1)
			{
				if (dup2(file->hd_pipe[0], 0) == -1)
					return (write_sys_error("dup2 failed"));
			}
			if (exit_code != 0)
				break ;
		}
		file = file->next;
	}
	return (exit_code);
}

int	create_extra_outfiles(t_token *cur_token, int ofile_count)
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
				if (ft_just_create_file(file->filename) == 1)
					return (1);
				ofile_count--;
			}
			file = temp;
		}
		temp = temp->next;
	}
	return (0);
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

int	ft_redirect(t_data *data, int **fd_pipes, int index)
{
	int		exit_code;
	int		ofile_count;
	t_token	*cur_token;
	t_file	*file;

	exit_code = 0;
	cur_token = get_cur_token(data, index);

	if (index != 0 && check_for_infile(cur_token) == 0)
		exit_code = dup_pipe_in(fd_pipes, index);
	else
		exit_code = process_infile(cur_token);

	if (exit_code != 0)
	{
		close_all_pipes(data, fd_pipes, (data->proc_count - 1));
		ft_free_int_doubleptr(fd_pipes);
		return (exit_code);
	}


	ofile_count = count_outfiles(cur_token);
	if (ofile_count > 1)
	{
		exit_code = create_extra_outfiles(cur_token, ofile_count);
		if (exit_code != 0)
			return (exit_code);
	}
	file = find_outfile(cur_token);


	
	if (index != (data->proc_count - 1) && file == NULL)
		exit_code = dup_pipe_out(fd_pipes, index);
	else if (file != NULL)
	{
		if (file->is_append == -1)
			exit_code = open_outfile(file->filename);
		else if (file->is_append == 1)
			exit_code = open_outfile_append(file->filename);
	}

	close_all_pipes(data, fd_pipes, (data->proc_count - 1));
	ft_free_int_doubleptr(fd_pipes);
	return (exit_code);
}
