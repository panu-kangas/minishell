#include "minishell.h"

int	is_relative_path(char *path)
{
	if (ft_strncmp(path, ".", 2) == 0 || ft_strncmp(path, "./", 2) == 0 \
		|| ft_strncmp(path, "..", 3) == 0 || ft_strncmp(path, "../", 3) == 0)
		return (1);
	else
		return (0);
}

char	*get_absolute_path(char *new_path, t_data *data)
{
	if (ft_strncmp(new_path, ".", 2) == 0 || ft_strncmp(new_path, "./", 3) == 0)
		return (ft_strdup(data->current_directory));
	else
		return (0); // WE NEED PARENT DIRECTORY, so full path - last dir
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
	if (ft_strncmp(new_path, ".", 2) == 0 || ft_strncmp(new_path, "..", 3) == 0 \
	|| ft_strncmp(new_path, "./", 3) == 0 || ft_strncmp(new_path, "../", 4) == 0)
		new_path = get_absolute_path(new_path, data);

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


	if (flag == 2)
	{
		if (is_relative_path(path) == 1)
			return (write_weird_cd_error("Permission denied")); // This will also change pwd, so the data->current_directory...
		else
			(write_error("cd", path, "Permission denied"));
	}


	return (0);
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

int	analyze_path(char *orig_path, t_data *data)
{
	int		flag;
	char	*path;

	flag = check_valid_path(orig_path);

	ft_printf("flag: %d\n", flag);

	if (flag == 0)
		return (-1);
	else if (flag == 2)
		return (write_error("cd", orig_path, "Not a directory"));
	else if (flag == 3)
			return (write_error("cd", orig_path, "Permission denied"));
	
	path = ft_strdup(orig_path);
	if (path == NULL)
		return (write_sys_error("malloc failed"));

	flag = check_all_permissions(path, data);
	return (flag);
}
