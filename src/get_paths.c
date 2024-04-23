#include "minishell.h"

void	free_paths(char **paths, int paths_count)
{
	int	i;

	i = paths_count - 1;
	while (i > -1)
		free(paths[i--]);
	free(paths);
}

int	add_slash(char **paths)
{
	int		i;
	int		paths_count;
	char	*temp;

	i = -1;
	paths_count = 0;
	while (paths[++i] != NULL)
		paths_count++;
	i = 0;
	while (paths[i] != NULL)
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		temp = NULL;
		if (paths[i] == NULL)
		{
			free_paths(paths, paths_count);
			return (write_sys_error("malloc failed"));
		}
		i++;
	}
	return (0);
}

char	**get_paths(t_env_lst *env_lst)
{
	char		**paths;
	t_env_lst	*temp;

	temp = check_if_var_exist(env_lst, "PATH");
	paths = ft_split(temp->value, ':');
	if (paths == NULL)
		return (NULL);
	if (add_slash(paths) != 0)
		return (NULL);
	return (paths);
}