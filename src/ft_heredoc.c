#include "minishell.h"

int	write_hd(char *hd_str, char *limiter, int *hd_pipe_fd)
{
	int	fd;
	int	str_len;
	int	limit_len;

	if (hd_str == NULL)
		return (0);
	if (pipe(hd_pipe_fd) < 0)
		return (write_sys_error("pipe failed"));
	fd = hd_pipe_fd[1];
	str_len = ft_strlen(hd_str);
	limit_len = ft_strlen(limiter);
	if (limit_len == 0)
		hd_str[str_len - 1] = '\0';
	else if (limit_len != 0)
		hd_str[str_len - limit_len - 1] = '\0';
	else if (str_len == 1 && limit_len == 0)
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

	i = ft_strlen(hd_str);
	limit_len = ft_strlen(limiter);
	if (--i == 0 && limit_len > 0)
		return (0);
	else if (i == 0 && limit_len == 0)
		return (1);
	else if (hd_str[i] == '\n' && hd_str[i - 1] == '\n' && limit_len == 0)
		return (1);
	else if (hd_str[i] == '\n' && line_len == 0 && limit_len == 0)
		return (1);
	i--;
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

void	expand_hd_content(t_env_lst *env_lst, char *hd_str) // NOT DONE
{
	if (hd_str == NULL)
		return ;
	if (ft_strchr(hd_str, '$'))
		expand_env_var(env_lst, "USER");
}

int	ft_heredoc(t_env_lst *env_lst, char *limiter, int *hd_pipe_fd)
{
	char	*hd_str;
	char	*read_line;
	int		line_len;

	hd_str = NULL;
	read_line = NULL;
	while (1)
	{
		read_line = readline("> ");
		line_len = ft_strlen(read_line);
		if (read_line == NULL) // With crtl + D, one extra line gets produced (compare to bash)
			break ;		
		hd_str = get_whole_hd_str(read_line, hd_str);
		if (hd_str == NULL)
			return (write_sys_error("malloc failed"));
		if (check_limiter(hd_str, limiter, line_len) == 1)
			break ;
	}
	expand_hd_content(env_lst, hd_str);  // NOT DONE
	return (write_hd(hd_str, limiter, hd_pipe_fd));
}


int	process_heredoc(t_data *data, t_env_lst *env_lst, int exit_status)
{
	t_token *cur_token;
	t_file	*cur_file;

	alter_termios(0);
	cur_token = data->tokens;
	while (cur_token != NULL)
	{
		cur_file = cur_token->files;
		while (cur_file != NULL)
		{
			if (cur_file->is_infile == 1 && cur_file->is_append == 1)
				exit_status = ft_heredoc(env_lst, cur_file->filename, cur_file->hd_pipe);
			if (exit_status != 0)
			{
				alter_termios(1);
				return (exit_status);
			}
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
	alter_termios(1);
	return (0);
}


/*

int	check_limiter(char *hd_str, char *limiter, int bytes)
{
	char	*temp;
	int		i;
	int		limit_len;

	i = ft_strlen(hd_str);
	limit_len = ft_strlen(limiter);
	if (--i == 0 && limit_len > 0)
		return (0);
	else if (i == 0 && limit_len == 0)
		return (1);
	else if (hd_str[i] == '\n' && hd_str[i - 1] == '\n' && limit_len == 0)
		return (1);
	else if (hd_str[i] == '\n' && bytes == 0)
		return (1);
	i--;
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



char	*get_str(char *hd_str, char *buf, int bytes)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	if (bytes == 0 && hd_str != NULL)
		return (hd_str);
	temp = malloc((ft_strlen(hd_str) + bytes + 1) * sizeof(char));
	if (temp == NULL)
	{
		free(hd_str);
		return (NULL);
	}
	while (hd_str != NULL && hd_str[j] != '\0')
		temp[i++] = hd_str[j++];
	free(hd_str);
	j = 0;
	while (j < bytes)
		temp[i++] = buf[j++];
	temp[i] = '\0';
	hd_str = ft_strdup(temp);
	free(temp);
	return (hd_str);
}


// IMPORTANT NOTE !!
// We need a "free all" -helper function here!
// Because now we are in a separate process, so all needs to be freed if something fails.
// For example: free data-struc, free env_var, close pipe

int	ft_heredoc(char *limiter, int *hd_pipe_fd, t_data *data, t_env_lst *env_lst)
{
	char	buf[100];
	char	*hd_str;
	int		bytes;

	bytes = 1;
	hd_str = NULL;
	ft_printf("> ");
	while (1)
	{
		if (bytes != 0 && hd_str != NULL && hd_str[ft_strlen(hd_str) - 1] == '\n')
			ft_printf("> ");
		bytes = read(0, buf, 100); // Use readline instead?? You get weird
		if (bytes == -1)
		{
			free(hd_str);
			free_env_lst(env_lst); // FOR TESTING, remove in the end
			ft_free_data(data, 0); // FOR TESTING, remove in the end
			return (write_sys_error("read failed"));
		}
		hd_str = get_str(hd_str, buf, bytes);
		if (hd_str == NULL)
			return (write_sys_error("malloc failed"));
		if (check_limiter(hd_str, limiter, bytes) == 1 || \
		(bytes == 0 && hd_str[0] == '\0'))
			break ;
	}
	return (write_hd(hd_str, limiter, bytes, hd_pipe_fd));
}





*/
