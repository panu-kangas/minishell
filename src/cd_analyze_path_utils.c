/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_analyze_path_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:19:50 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 13:19:53 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_relative_path(char *path)
{
	int	i;

	i = ft_strlen(path) - 1;
	if (path == NULL || path[0] == '/')
		return (0);
	if (ft_strncmp(path, ".", 2) == 0 || ft_strncmp(path, "./", 3) == 0)
		return (1);
	else if (ft_strncmp(path, "..", 3) == 0 || ft_strncmp(path, "../", 4) == 0)
		return (2);
	else if (ft_strncmp(path, "./", 2) == 0)
		return (5);
	else if (ft_strncmp(path, "../", 3) == 0)
		return (6);
	else if (ft_strnstr(path, "/./", ft_strlen(path)) != NULL \
	|| ft_strnstr(path, "/../", ft_strlen(path)) != NULL)
		return (7);
	else if (i > 0 && path[i - 1] == '/' && path[i] == '.')
		return (8);
	else if (i > 1 && path[i - 2] == '/' \
	&& path[i - 1] == '.' && path[i] == '.')
		return (9);
	else if (path[0] != '/')
		return (10);
	return (0);
}

static int	free_paths_cd(char **paths, int paths_count)
{
	int	i;

	i = paths_count - 1;
	while (i >= 0)
		free(paths[i--]);
	free(paths);
	return (-2);
}

int	add_slash_cd(char **paths)
{
	int		i;
	int		paths_count;
	char	*temp;

	i = -1;
	paths_count = 0;
	while (paths[++i] != NULL)
		paths_count++;
	temp = paths[0];
	paths[0] = ft_strjoin("/", paths[0]);
	free(temp);
	if (paths[0] == NULL)
		return (free_paths_cd(paths, paths_count));
	i = 0;
	while (paths[i] != NULL)
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		temp = NULL;
		if (paths[i] == NULL)
			return (free_paths_cd(paths, paths_count));
		i++;
	}
	return (0);
}

char	*join_path_to_cur_dir(char *temp_cur_dir, char *path)
{
	char	*temp;

	temp = temp_cur_dir;
	temp_cur_dir = ft_strjoin(temp_cur_dir, "/");
	free(temp);
	if (temp_cur_dir == NULL)
		return (NULL);
	temp = temp_cur_dir;
	temp_cur_dir = ft_strjoin(temp_cur_dir, path);
	free(temp);
	if (temp_cur_dir == NULL)
		return (NULL);
	return (temp_cur_dir);
}

char	*update_cur_dir(char *temp_cur_dir, char *path)
{
	int		i;

	i = ft_strlen(temp_cur_dir) - 1;
	if (is_relative_path(path) == 1)
		return (temp_cur_dir);
	else if (is_relative_path(path) == 2)
	{
		if (i > 0 && temp_cur_dir[i] == '/')
		{
			while (temp_cur_dir[i] == '/')
				i--;
		}
		while (i > 0 && temp_cur_dir[i] != '/')
			i--;
		if (i > 0)
			temp_cur_dir[i] = '\0';
		return (temp_cur_dir);
	}
	else
		return (join_path_to_cur_dir(temp_cur_dir, path));
}
