#include "minishell.h"

int	is_relative_path(char *path)
{
	int	i;

	i = ft_strlen(path) - 1;

	if (path == NULL)
		return (0);
	if (ft_strncmp(path, ".", 2) == 0 || ft_strncmp(path, "./", 3) == 0)
		return (1);
	else if (ft_strncmp(path, "..", 3) == 0 || ft_strncmp(path, "../", 4) == 0)
		return (2);
	else if (ft_strncmp(path, "./", 2) == 0)
		return (5);
	else if (ft_strncmp(path, "../", 3) == 0)
		return (6);
	else if (ft_strnstr(path, "/./", ft_strlen(path)) != 0 || ft_strnstr(path, "/../", ft_strlen(path)) != 0)
		return (7);
	else if (i > 0 && path[i - 1] == '/' && path[i] == '.')
		return (8);
	else if (i > 1 && path[i - 2] == '/' && path[i - 1] == '.' && path[i] == '.')
		return (7);
	else if (path[0] != '/')
		return (8);
	return (0);
}

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

char	*update_cur_dir(char *temp_cur_dir, char *path)
{
	int		i;
	char	*temp;

	i = ft_strlen(temp_cur_dir) - 1;

	if (is_relative_path(path) == 1)
		return (temp_cur_dir);
	else if (is_relative_path(path) == 2)
	{
		if (temp_cur_dir[i] == '/')
		{
			while (temp_cur_dir[i] == '/')
				i--;
		}
		while (temp_cur_dir[i] != '/')
			i--;
		if (i != 0)
			temp_cur_dir[i] = '\0';
		return (temp_cur_dir);
	}
	else
	{
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
}

 // ONE EDGE CASE: if your deep in a "No such file", and wanna backtrack using ../../../ path or even ../../../../minishell
 // SOLUTION: add a check: if cur_dir_flag == 1 && is_relative_path == 6
 // make a function that builds the "final directory" using split_path.
 // if final directory is valid --> return -1, else throw error

int	analyze_path(char *path, t_data *data)
{
	int		valid_path_flag;
	int		cur_dir_flag;
	int		path_flag;
	int		i;
	char	**split_cur_dir;
	char	**split_path;


	valid_path_flag = check_valid_path(path);

	ft_printf("FLAG 1: %d\n", valid_path_flag);

	if (valid_path_flag == 0)
		return (-1);
	else if (valid_path_flag == 1 && is_relative_path(path) == 0)
		return (write_error("cd", path, "No such file or directory"));
	else if (valid_path_flag == 2)
		return (write_error("cd", path, "Not a directory"));
	else if (valid_path_flag == 3)
		return (write_error("cd", path, "Permission denied"));

	split_cur_dir = ft_pipex_split(data->current_directory, '/');
	if (split_cur_dir == NULL)
		return (write_sys_error("malloc failed"));
	cur_dir_flag = check_cur_dir_permissions(split_cur_dir);
	if (cur_dir_flag != -2)
		ft_free_doubleptr(split_cur_dir);
	if (cur_dir_flag < 0)
		return (write_sys_error("malloc failed"));

	ft_printf("FLAG 2: %d\n", cur_dir_flag);

	if (is_relative_path(path) == 1 && cur_dir_flag == 3)
	{
		if (check_parent_dir_permissions(data->current_directory) == 0)
			return (write_error("cd", path, "Permission denied"));
	}
	else if (is_relative_path(path) == 2 && cur_dir_flag == 3)
	{
		path_flag = check_parent_dir_permissions(data->current_directory);
		if (path_flag == 0)
			return (-2);
	} // add edge check here !! 
	else if (is_relative_path(path) > 6 && cur_dir_flag == 1)
		return (write_error("cd", path, "No such file or directory"));
	else if (is_relative_path(path) != 0 && (cur_dir_flag == 3 || cur_dir_flag == 1))
	{
		if (chdir(path) != 0)
		{
			if (cur_dir_flag == 1)
				return (write_error("cd", path, "No such file or directory"));
			else if (cur_dir_flag == 3)
				return (write_error("cd", path, "Permission denied"));
		}
		else
			return (-3);
	}
	else if (cur_dir_flag == 0)
	{
		split_path = ft_split(path, '/');
		if (split_path == NULL)
			return (write_sys_error("malloc failed"));
		i = 0;
		errno = 0;
		while (split_path[i] != NULL)
		{			
			if (chdir(split_path[i]) != 0)
			{
				if (errno == EACCES && is_relative_path(split_path[i + 1]) == 2)
				{
					errno = 0;
					i++;
				}
				else if (errno == EACCES && is_relative_path(split_path[i + 1]) == 1)
				{
					free(split_path[i + 1]);
					split_path[i + 1] = ft_strdup(split_path[i]);
					if (split_path[i + 1] == NULL)
						return (write_sys_error("malloc failed"));
				}
				else
					break ;
			}
			i++;
		}
		if (split_path[i] != NULL)
		{
			ft_free_doubleptr(split_path);
			if (errno == ENOENT)
				path_flag = 1;
			else if (errno == EACCES)
				path_flag = 3;
			if (chdir(data->current_directory) != 0)
				return (write_sys_error("chdir failed"));
			if (path_flag == 1)
				return (write_error("cd", path, "No such file or directory"));
			else if (path_flag == 3)
				return (write_error("cd", path, "Permission denied"));
		}
		else
			return (-3);
	}

	return (-1);
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

/* int	check_given_path_permissions(t_data *data, char **split_path)
{
	int		i;
	int		flag;
	char	*path_to_check;
	char	*temp;

	i = 0;
	flag = 0;
	path_to_check = NULL;
	while (split_path[i] != NULL)
	{
		temp = path_to_check;
		if (path_to_check == NULL)
		{
			path_to_check = ft_strdup(data->current_directory);
			if (path_to_check == NULL)
				return (-1);
			temp = path_to_check;
			path_to_check = get_new_path(path);
		}
		else
			path_to_check = ft_strjoin(path_to_check, split_path[i]);
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
} */


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


/* int	check_two_folders_up(char *path)
{
	int		i;
	int		flag;
	int		counter;
	char	*temp;

	temp = ft_strdup(path);
	if (temp == NULL)
		return (-1);
	i = ft_strlen(temp) - 1;
	counter = 1;
	while (i != 0)
	{
		if (temp[i] == '/')
		{
			if (temp[i + 1] != '\0' && temp[i + 1] != '/')
			{
				if (counter == 0)
					break ;
				else
					counter--;
			}
		}
		i--;
	}
	if (i != 0)
		temp[i] = '\0';
	flag = check_valid_path(temp);
	free(temp);
	return (flag);
} */



/*	else if (is_relative_path(path) > 4)
	{
		split_path = ft_split(path, '/');
		if (split_path == NULL)
			return (write_sys_error("malloc failed"));
		path_flag = check_given_path_permissions(data, split_path);
		if (path_flag < 0)
			return (write_sys_error("malloc failed"));
		else if (path_flag == 4)
			return (-3);
		ft_printf("FINAL FLAG: %d\n", path_flag);
		// check flag value and act accordingly
	} */

/*
int	check_given_path_permissions(t_data *data, char **split_path)
{
	int		i;
	int		flag;
	char	*temp_cur_dir;
//	char	*temp;

	i = 0;
	flag = 0;
	temp_cur_dir = ft_strdup(data->current_directory);
	if (temp_cur_dir == NULL)
		return (-1);
	while (split_path[i] != NULL)
	{
		errno = 0;

		ft_printf("temp-cur-dir: %s\n", temp_cur_dir);

		if (chdir(split_path[i]) != 0)
		{
			flag = 1;
			if (errno == EACCES)
			{
				if (split_path[i + 1] == NULL || \
				is_relative_path(split_path[i + 1]) != 2)
					return (3);
				else if (is_relative_path(split_path[i]) != 2 && is_relative_path(split_path[i + 1]) == 2 \
				&& is_relative_path(split_path[i + 2]) == 2)
					return (3);
				else if (is_relative_path(split_path[i]) == 2 && is_relative_path(split_path[i + 1]) == 2)
					return (3);
			}
			else if (errno == ENOENT) // just a test
			{
				ft_printf("ENOENT\n");
			}
		}
		temp_cur_dir = update_cur_dir(temp_cur_dir, split_path[i]);
		if (temp_cur_dir == NULL)
			return (-1);
		i++;
	}
	free(temp_cur_dir);
	if (flag == 0)
		return (4);
	return (0);
} */
