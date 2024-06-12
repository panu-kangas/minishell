/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:35:35 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/12 15:21:03 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_hd(char *hd_str, char *lim, int fd)
{
	int	str_len;
	int	lim_len;

	if (hd_str == NULL)
	{
		write(fd, "\0", 1);
		return (0);
	}
	else if (ft_strlen(hd_str) > 60000)
	{
		free(hd_str);
		return (write_error(NULL, "heredoc error", "heredoc input too long"));
	}
	str_len = ft_strlen(hd_str);
	lim_len = ft_strlen(lim);
	if (search_for_limiter(hd_str, lim, str_len, lim_len) == 1)
		hd_str[str_len - lim_len - 1] = '\0';
	else if (str_len == 1 && lim_len == 0)
		hd_str[0] = '\0';
	write(fd, hd_str, ft_strlen(hd_str));
	free(hd_str);
	return (0);
}

int	check_limiter(char *hd_str, char *limiter, int line_len)
{
	char	*temp;
	int		i;
	int		limit_len;

	if (hd_str == NULL)
		return (0);
	i = ft_strlen(hd_str);
	limit_len = ft_strlen(limiter);
	if (--i == 0 && limit_len > 0)
		return (0);
	else if (i == 0 && limit_len == 0)
		return (1);
	else if (hd_str[i] == '\n' && hd_str[i - 1] == '\n' && limit_len == 0)
		return (1);
	else if (hd_str[i--] == '\n' && line_len == 0 && limit_len == 0)
		return (1);
	while (i > 0 && hd_str[i] != '\n')
		i--;
	if (hd_str[i] == '\n')
		i++;
	temp = &hd_str[i];
	if (ft_strncmp(temp, limiter, limit_len) == 0 && temp[limit_len] == '\n')
		return (1);
	else
		return (0);
}

char	*get_whole_hd_str(char *read_line, char *hd_str)
{
	int		i;
	int		j;
	char	*temp;

	temp = malloc(ft_strlen(read_line) + ft_strlen(hd_str) + 2);
	if (temp == NULL)
	{
		free(hd_str);
		free(read_line);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (hd_str != NULL && hd_str[i] != '\0')
		temp[j++] = hd_str[i++];
	free(hd_str);
	i = 0;
	while (read_line != NULL && read_line[i] != '\0')
		temp[j++] = read_line[i++];
	temp[j] = '\n';
	temp[j + 1] = '\0';
	free(read_line);
	return (temp);
}

int	ft_heredoc(t_env *env_lst, char *limiter, int *hd_pipe_fd, int flag)
{
	char	*hd_str;
	char	*read_line;
	int		line_len;

	hd_str = NULL;
	line_len = 0;
	while (check_limiter(hd_str, limiter, line_len) != 1)
	{
		read_line = readline("> ");
		line_len = ft_strlen(read_line);
		if (read_line == NULL)
			break ;
		hd_str = get_whole_hd_str(read_line, hd_str);
		if (hd_str == NULL)
			return (write_sys_error("malloc failed"));
	}
	if (g_signal_marker == 2)
		return (free_hd_str(hd_str, 1));
	if (ft_strlen(hd_str) <= 60000 && pipe(hd_pipe_fd) < 0)
		return (write_sys_error("pipe failed"));
	if (hd_str != NULL && ft_strchr(hd_str, '$') != NULL && flag == 0)
		return (write_expanded_hd(env_lst, hd_str, limiter, hd_pipe_fd[1]));
	else
		return (write_hd(hd_str, limiter, hd_pipe_fd[1]));
}

int	process_heredoc(t_data *data, t_env *env_lst, int exit_status)
{
	t_token	*cur_token;
	t_file	*cur_file;

	alter_termios(0);
	cur_token = data->tokens;
	while (cur_token != NULL)
	{
		cur_file = cur_token->files;
		while (cur_file != NULL)
		{
			if (cur_file->is_infile == 1 && cur_file->is_append == 1)
			{
				exit_status = ft_heredoc(env_lst, cur_file->filename, \
				cur_file->hd_pipe, cur_file->quoted_heredoc);
			}
			if (exit_status != 0)
				break ;
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
	alter_termios(1);
	return (exit_status);
}
