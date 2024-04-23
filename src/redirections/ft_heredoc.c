#include "minishell.h"

int	write_hd(char *str, char *limiter, int bytes)
{
	int	fd;
	int	str_len;
	int	heredoc_fd[2];

	if (pipe(heredoc_fd) == -1)
		return (write_sys_error("pipe failed"));
	fd = heredoc_fd[1];
	str_len = ft_strlen(str);
	if (ft_strlen(limiter) == 0 && bytes != 0)
		str[str_len - 1] = '\0';
	else if (bytes != 0)
		str[str_len - ft_strlen(limiter) - 1] = '\0';
	write(fd, str, ft_strlen(str));
	free(str);
	if (dup2(heredoc_fd[0], 0) == -1)
		return (write_sys_error("dup2 failed"));
	close(heredoc_fd[1]);
	close(heredoc_fd[0]);
	return (0);
}

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

int	ft_heredoc(char *limiter)
{
	char	buf[100];
	char	*hd_str;
	int		bytes;

	bytes = 1;
	hd_str = NULL;
	ft_printf(">");
	while (1)
	{
		if (bytes != 0 && hd_str != NULL && hd_str[ft_strlen(hd_str) - 1] == '\n')
			ft_printf(">");
		bytes = read(0, buf, 100);
		if (bytes == -1)
		{
			free(hd_str);
			return (write_sys_error("read failed"));
		}
		hd_str = get_str(hd_str, buf, bytes);
		if (hd_str == NULL)
			return (write_sys_error("malloc failed"));
		if (check_limiter(hd_str, limiter, bytes) == 1 || \
		(bytes == 0 && hd_str[0] == '\0'))
			break ;
	}
	return (write_hd(hd_str, limiter, bytes));
}
