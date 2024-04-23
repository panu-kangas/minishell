/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 08:45:30 by tsaari            #+#    #+#             */
/*   Updated: 2024/02/14 10:50:14 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_free(char **ptr, char **ptr2)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
	if (ptr2 != NULL && *ptr2 != NULL)
	{
		free(*ptr2);
		*ptr2 = NULL;
	}
	return (NULL);
}

static char	*read_file_until_nl(int fd, char *buffer)
{
	char	readed[BUFFER_SIZE + 1];
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	if (!buffer)
		buffer = ft_calloc(1, 1);
	while (bytes_read > 0)
	{
		bytes_read = read(fd, readed, BUFFER_SIZE);
		if (bytes_read == -1 || (!buffer) || \
		(bytes_read == 0 && ft_strlen(buffer) == 0))
			return (ft_free(&buffer, NULL));
		readed[bytes_read] = '\0';
		temp = buffer;
		buffer = ft_strjoin (buffer, readed);
		ft_free(&temp, NULL);
		if (!buffer)
			return (ft_free(&buffer, NULL));
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;
	char		*temp;
	size_t		newline;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_file_until_nl(fd, buffer);
	if (!buffer)
		return (ft_free(&buffer, NULL));
	newline = (ft_strchr(buffer, '\n') - buffer);
	line = ft_substr(buffer, 0, newline + 1);
	if (!line)
		return (ft_free(&buffer, &line));
	temp = buffer;
	buffer = ft_substr(buffer, newline + 1, ft_strlen(buffer) - newline);
	free(temp);
	if (!buffer && !line)
		return (ft_free(&buffer, &line));
	return (line);
}
