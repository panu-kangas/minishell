#include "minishell.h"

t_env_lst	*ft_unset(t_env_lst *env_lst, char *var_name)
{
	t_env_lst	*temp;
	t_env_lst	*temp_prev;

	temp = env_lst;
	temp_prev = env_lst;
	if (temp->name == NULL)
		return (env_lst);
	if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
	{
		env_lst = env_lst->next;
		delete_env_node(temp);
		return (env_lst);
	}
	while (temp->next != NULL)
	{
		temp_prev = temp;
		temp = temp->next;
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
		{
			temp_prev->next = temp->next;
			delete_env_node(temp);
			break ;
		}
	}
	return (env_lst);
}


t_env_lst	*check_if_var_exist(t_env_lst *env_lst, char *var_name)
{
	while (env_lst != NULL)
	{
		if (ft_strncmp(env_lst->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (env_lst);
		env_lst = env_lst->next;
	}
	return (NULL);
}


// env_lst check in the beginning!
// I mean: if env_lst->name == NULL, then just copy the name and value to the first node.

int	ft_export(t_env_lst *env_lst, char *new_env_var)
{
	int			flag;
	char		*var_name;
	t_env_lst	*temp;

	flag = 0;
	var_name = get_var_name(new_env_var);
	if (var_name == NULL)
		return (ERR_STAT);
	temp = check_if_var_exist(env_lst, var_name);
	free(var_name);
	if (temp == NULL || temp->is_global == 1)
	{
		if (ft_strchr(new_env_var, '=') == NULL)
			flag = process_null_value_env_node(env_lst, new_env_var);
		else
			flag = process_global_env_node(env_lst, new_env_var);
	}
	else
		temp->is_global = 1;
	if (flag == ERR_STAT)
		return (ERR_STAT);
	else
		return (0);
}
