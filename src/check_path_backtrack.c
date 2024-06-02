#include "minishell.h"

/*

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

void	get_rid_of_extra(char *cur_dir, char **split_path)
{
	int	i;
	static int	counter;

	i = ft_strlen(cur_dir) - 1;
	while (cur_dir[i] == '/' || cur_dir[i] == '.')
		i--;
	cur_dir[i + 1] = '\0';
}

char	*build_final_path(t_data *data, char **split_path)
{
	int		i;
	char	*cur_dir;
	char	*cur_dir_storage;

	if (data->cd_first_time == 0)
		cur_dir = ft_strdup(data->current_directory);
	else
		cur_dir = ft_strdup(cur_dir_storage);
	if (cur_dir == NULL)
		return (NULL);
	get_rid_of_extra(cur_dir, split_path);
	i = -1;
	while (split_path[++i] != NULL)
		update_cur_dir(cur_dir, split_path[i]);
	return (cur_dir);
}

int	check_first_try(t_data *data, char **split_path)
{
	static int	first_time;
	int			is_relative;
	char		*final_path;

	is_relative = is_relative_path(data->current_directory);
	final_path = build_final_path(data, split_path);
	if (first_time == 0 && is_relative == 0)
	{
		first_time = 1;
		free(final_path);
		ft_free_doubleptr(split_path);
		return (1);
	}
	else if (first_time == 1 && check_valid_path(final_path) == 0)
	{
		first_time = 0;
		free(final_path);
		return (0);
	}
	free(final_path);
	return (0);
}

int	check_path_backtrack(t_data *data, char *path)
{
	int		chdir_flag;
	char	*final_path;
	char	**split_path;

	split_path = ft_pipex_split(path, '/');
	if (split_path == NULL)
		return (write_sys_error("malloc failed"));
	if (check_first_try(data, split_path) == 1)
		return (write_error("cd", path, "No such file or directory"));
	final_path = build_final_path(data, split_path);
	ft_free_doubleptr(split_path);
	if (final_path == NULL)
		return (write_sys_error("malloc failed"));
	else if (check_valid_path(final_path) == 0)
	{
		chdir_flag = chdir(final_path);
		free(final_path);
		if (chdir_flag == -1)
			return (write_sys_error("chdir failed"));
		return (-3);
	}
	free(final_path);
	if (is_relative_path(path) == 2)
		return (-1);
	return (write_error("cd", path, "No such file or directory"));
}

*/