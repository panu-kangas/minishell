#include "minishell.h"

int	is_relative_path(char *path)
{
	if (path == NULL)
		return (0);
	if (ft_strncmp(path, ".", 2) == 0)
		return (1);
	else if (ft_strncmp(path, "..", 3) == 0)
		return (2);
	else if (ft_strncmp(path, "./", 2) == 0 || ft_strncmp(path, "../", 3) == 0)
		return (3);
	else if (ft_strnstr(path, "./", ft_strlen(path)) != 0 || ft_strnstr(path, "../", ft_strlen(path)) != 0)
		return (4);
	else
		return (0);
}

/*

char *build_first_path(t_data *data, char *path_to_check, char *path_to_add)
{
	char	*temp;

	if (is_relative_path(path_to_add) == 1)
		return (ft_strdup(data->current_directory));
	else if (is_relative_path(path_to_add) == 2)
	{
		path_to_check = ft_strdup(data->current_directory);
		if (path_to_check == NULL)
			return (NULL);
		temp = ft_strrchr(path_to_check, '/');
		if (temp == NULL)
			return (NULL); // is this right...? What should I to do if no / was found?
		*temp = '\0';
		return (path_to_check);
	}
	else

}

char	*build_path(t_data *data, char *path_to_check, char *path_to_add, int i)
{
	if (i == 0)
		return (build_first_path(data, path_to_check, path_to_add));

	


}

int	check_all_permissions(char **split_path, t_data *data) // THIS IS IN PROCESS !!!
{
	int		i;
	int		flag;
	char	*path_to_check;
	char	*temp;

	i = 0;
	flag = 0;
	path_to_check == NULL;
	while (1)
	{
		temp = path_to_check;
		path_to_check = build_path(data, path_to_check, split_path[i], i);
		free(temp);
		if (path_to_check == NULL)
			return (write_sys_error("malloc error"));
		flag = check_valid_path(path_to_check);
		free(path_to_check);
		if (flag != 0)
			break ;
		i++;
	}

	if (flag == 3)
	{
		if (is_relative_path(path) > 0)
			return (write_weird_cd_error("Permission denied")); // This will also change pwd, so the data->current_directory...
		else
			(write_error("cd", path, "Permission denied"));
	}


	return (0);
}

*/

int	free_paths_cd(char **paths, int paths_count)
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
	while (split_cur_dir[i] != NULL)
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
		if (flag != 0)
			break ;
		i++;
	}
	free(path_to_check);
	return (flag);
}

int	check_valid_path(char *path)
{
	struct stat statbuf;

	if (path == NULL)
		return (0);
	if (stat(path, &statbuf) == -1)
		return (1); // (write_error("cd", path, "No such file or directory"));
	if (stat(path, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode) == 0)
			return (2); // (write_error("cd", path, "Not a directory"));
	}
	if (access(path, X_OK) == -1)
		return (3); //(write_error("cd", path, "Permission denied"));
	return (0);
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
	while (1)
	{
		if (temp[i] == '/') // is there a problem with root folder (name just /)
		{
			if (temp[i + 1] != '\0' && temp[i + 1] != '/')
				break ;
		}
		i--;
		if (i == 0)
			break ;
	}
	if (i != 0)
		temp[i] = '\0';
	flag = check_valid_path(temp);
	free(temp);
	return (flag);
}

int	analyze_path(char *path, t_data *data)
{
	int		flag;
//	char	**split_path;
	char	**split_cur_dir;

	flag = check_valid_path(path);
	ft_printf("FLAG 1: %d\n", flag);

	// if I'm under a permission fail folder and try to move onwards to an existing dir
	// I get a flag 0... this needs to be tracked!!

	if (flag == 0 && is_relative_path(path) == 0)
		return (-1);
	else if (flag == 1 && is_relative_path(path) == 0)
		return (write_error("cd", path, "No such file or directory"));
	else if (flag == 2)
		return (write_error("cd", path, "Not a directory"));
	else if (flag == 3)
		return (write_error("cd", path, "Permission denied"));

	split_cur_dir = ft_split(data->current_directory, '/');
	if (split_cur_dir == NULL)
		return (write_sys_error("malloc failed"));
	flag = check_cur_dir_permissions(split_cur_dir);
	if (flag != -2)
		ft_free_doubleptr(split_cur_dir);
	if (flag < 0)
		return (write_sys_error("malloc failed"));

	ft_printf("FLAG 2: %d\n", flag);

	if (flag == 3 && is_relative_path(path) == 1)
	{
		if (check_parent_dir_permissions(data->current_directory) == 0)
			return (write_error("cd", path, "Permission denied"));
		else
		{
			write_weird_cd_error("Permission denied"); // data->cur_dir update?
			return (0);
		}
	}
	else if (flag == 3 && is_relative_path(path) == 2)
	{
		flag = check_parent_dir_permissions(data->current_directory);
		if (flag == 0)
			return (-2);
		else if (flag == 1)
		{
			write_weird_cd_error("Permission denied"); // data->cur_dir update?
			return (0);
		}
		else if (flag == 3)
			return (write_error("cd", path, "Permission denied"));
	}
	// if path does not begin with .. and parent has Permission denied, just throw regular Permission-error
	// if path does not begin with .. and parent has No such file, just throw regular No file-error
	// if path begins with .. we need to check how the path goes to get the correct behaviour
	// --> you can backtrack out of No such file but NOT from Permissions denied subfolder


	if (flag == 0) // JUST A TEST
		flag = -1;

/*	split_path = ft_split(path, '/');
	if (split_path == NULL)
		return (write_sys_error("malloc failed"));
	flag = check_given_path_permissions();
	ft_free_doubleptr(split_path); */

	return (flag);
}



/*

int	is_relative_path(char *path)
{
	if (ft_strncmp(path, ".", 2) == 0 || ft_strncmp(path, "..", 3) == 0)
		return (1);
	else if (ft_strncmp(path, "./", 2) == 0 || ft_strncmp(path, "../", 3) == 0)
		return (2);
	else if (ft_strnstr(path, "./", ft_strlen(path)) == 0 || ft_strnstr(path, "../", ft_strlen(path)) == 0)
		return (3);
	else
		return (0);
}

char	*get_absolute_path(char *new_path, t_data *data)
{
	if (ft_strncmp(new_path, ".", 2) == 0)
	{
		free(new_path);
		return (ft_strdup(data->current_directory));
	}
	else
	{
		free(new_path);
		return (0); // WE NEED PARENT DIRECTORY, so full path - last dir
	}
}

void	build_path(char *path)
{
	int	i;

	i = 0;
	while (path[i] != '\0')
	{
		while (path[i] != '/')
		{
			// if is_relative_path returns 2
		}
	}
}

char	*extract_directory(char *path, int counter, t_data *data)
{
	int		i;
	char	*new_path;

	i = 0;
	while (path[i] != '\0')
	{
		if (path[i] == '/')
		{
			if (counter == 0)
				break ;
			else
				counter--;
		}
		i++;
	}
	new_path = ft_substr(path, 0, i);
	if (new_path == NULL)
		return (NULL);
	
	if (is_relative_path(new_path) == 1)
		new_path = get_absolute_path(path, data);
	else if (is_relative_path(path) > 1)
		build_path(new_path);

	return (new_path);
}

void	trim_extra_fslash(char *path)
{
	int	i;
	int	final_i;
	int	copy_start;
	int	len;

	i = 0;
	final_i = 0;
	len = 0;
	while (path[i] != '\0')
	{
		if (path[i] == '/' && path[i + 1] == '/')
		{
			if (final_i != 0)
				path[final_i++] = '/';
			while (path[i] == '/')
				i++;
			copy_start = i;
			len = 0;
			while (path[i] != '/' && path[i] != '\0')
			{
				i++;
				len++;
			}
			i = copy_start;
			while (len > 0)
			{
				path[final_i++] = path[i++];
				len--;
			}
		}
		else
			path[final_i++] = path[i++];
	}
	path[final_i] = '\0'; 
}

int	check_all_permissions(char *path, t_data *data) // THIS IS IN PROCESS !!!
{
	int		counter;
	int		flag;
	char	*path_to_check;

	counter = 0;
	trim_extra_fslash(path);
	while (1)
	{
		path_to_check = extract_directory(path, counter, data);
		if (path_to_check == NULL)
			return (write_sys_error("malloc error"));
		flag = check_valid_path(path_to_check);
		free(path_to_check);
		if (flag != 0)
			break ;
		counter++;
	}

	if (flag == 3)
	{
		if (is_relative_path(path) > 0)
			return (write_weird_cd_error("Permission denied")); // This will also change pwd, so the data->current_directory...
		else
			(write_error("cd", path, "Permission denied"));
	}


	return (0);
}

*/