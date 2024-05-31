#include "minishell.h"

int	check_valid_path(char *path)
{
	struct stat	statbuf;

	if (path == NULL)
		return (0);
	if (stat(path, &statbuf) == -1)
		return (1);
	if (stat(path, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 0)
			return (2);
	}
	if (access(path, X_OK) == -1)
		return (3);
	return (0);
}

int	check_cur_dir_permissions(char **split_cur_dir)
{
	int		i;
	int		flag;
	char	*path_to_check;
	char	*temp;

	i = 0;
	flag = 0;
	path_to_check = NULL;
	if (add_slash_cd(split_cur_dir) == -2)
		return (-2);
	while (split_cur_dir[i] != NULL && flag == 0)
	{
		temp = path_to_check;
		if (path_to_check == NULL)
			path_to_check = ft_strdup(split_cur_dir[0]);
		else
			path_to_check = ft_strjoin(path_to_check, split_cur_dir[i]);
		free(temp);
		if (path_to_check == NULL)
			return (-1);
		flag = check_valid_path(path_to_check);
		i++;
	}
	free(path_to_check);
	return (flag);
}

int	check_parent_dir_permissions(char *path)
{
	int		i;
	int		flag;
	char	*temp;

	temp = ft_strdup(path);
	if (temp == NULL)
		return (-1);
	i = ft_strlen(temp) - 1;
	while (i != 0)
	{
		if (temp[i] == '/')
		{
			if (temp[i + 1] != '\0' && temp[i + 1] != '/')
				break ;
		}
		i--;
	}
	if (i != 0)
		temp[i] = '\0';
	flag = check_valid_path(temp);
	free(temp);
	return (flag);
}

int	test_whole_path(char **split_path, int *path_flag, int i)
{
	while (split_path[++i] != NULL)	
	{
		errno = 0;
		if (chdir(split_path[i]) != 0)
		{
			if (errno == EACCES && is_relative_path(split_path[i + 1]) == 2)
				i++;
			else if (errno == EACCES && is_relative_path(split_path[i + 1]) == 1)
			{
				free(split_path[i + 1]);
				split_path[i + 1] = ft_strdup(split_path[i]);
				if (split_path[i + 1] == NULL)
				{
					ft_free_doubleptr(split_path);
					return (write_sys_error("malloc failed"));
				}
			}
			else
				break ;
		}
	}
	*path_flag = errno;
	if (split_path[i] == NULL)
		return (0);
	else
		return (1);
}
