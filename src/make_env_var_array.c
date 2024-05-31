#include "minishell.h"

int	env_lstsize_all(t_env_lst *lst)
{
	int	size;

	size = 0;
	while (lst != NULL)
	{
		if (lst->is_global == 1)
			size++;
		lst = lst->next;
	}
	return (size);
}

char	*get_full_env_str(t_env_lst *node)
{
	char 	*full_str;
	char	*temp;

	full_str = ft_strdup(node->name);
	if (full_str == NULL)
		return (NULL);
	if (node->value != NULL)
	{
		temp = full_str;
		full_str = ft_strjoin(temp, "=");
		free(temp);
		if (full_str == NULL)
			return (NULL);
		temp = full_str;
		full_str = ft_strjoin(full_str, node->value);
		free(temp);
		if (full_str == NULL)
			return (NULL);
	}
	return (full_str);
}

char	**make_env_var_array(t_env_lst *env_lst)
{
	char		**env_var_arr;
	t_env_lst	*temp;
	int			i;

	env_var_arr = malloc((env_lstsize_all(env_lst) + 1) * sizeof(char *));
	if (env_var_arr == NULL)
		return (NULL); // err_status?
	temp = env_lst;
	i = 0;
	while (temp != NULL)
	{
		if (temp->is_global == 1)
		{
			env_var_arr[i] = get_full_env_str(temp);
			if (env_var_arr[i] == NULL)
			{
				ft_free_doubleptr(env_var_arr);
				return (NULL);
			}
			i++;
		}
		temp = temp->next;
	}
	env_var_arr[i] = NULL;
	return (env_var_arr);
}
