#include "minishell.h"

void	get_parent_dir_path(char *cur_dir)
{
	int	i;

	i = ft_strlen(cur_dir) - 1;
	if (i <= 0)
		return ;
	if (i >= 0 && cur_dir[i] == '/')
	{
		while (i >= 0 && cur_dir[i] == '/')
			i--;
	}
	while (i >= 0 && cur_dir[i] != '/')
		i--;
	if (i > 0)
		cur_dir[i] = '\0';
}

void	get_rid_of_extra(char *cur_dir)
{
	int	i;

	i = ft_strlen(cur_dir) - 1;
	while (cur_dir[i] == '/' || cur_dir[i] == '.')
		i--;
	cur_dir[i + 1] = '\0';
}

char	*build_final_path(t_data *data, char **split_path)
{
	int		i;
	char	*cur_dir;

	i = 0;
	cur_dir = ft_strdup(data->current_directory);
	if (cur_dir == NULL)
		return (NULL);
	get_rid_of_extra(cur_dir);
	while (split_path[i] != NULL)
	{
		if (is_relative_path(split_path[i]) == 2)
			get_parent_dir_path(cur_dir);
		else if (is_relative_path(split_path[i]) != 1)
		{
			cur_dir = join_path_to_cur_dir(cur_dir, split_path[i]);
			if (cur_dir == NULL)
				return (NULL);
		}
		i++;
	}
	return (cur_dir);
}

int	check_path_backtrack(t_data *data, char *path) // On first time should throw error, then after that back tracking is possible
{
	int		path_flag;
	char	*final_path;
	char	**split_path;

	split_path = ft_pipex_split(path, '/');
	if (split_path == NULL)
		return (write_sys_error("malloc failed"));
	final_path = build_final_path(data, split_path);
	ft_free_doubleptr(split_path);
	if (final_path == NULL)
		return (write_sys_error("malloc failed"));
	path_flag = check_valid_path(final_path);
	if (path_flag == 0)
	{
		path_flag = chdir(final_path);
		free(final_path);
		if (path_flag == -1)
			return (write_sys_error("chdir failed"));
		return (-3);
	}
	else
	{
		free(final_path);
		return (write_error("cd", path, "No such file or directory"));
	}
}