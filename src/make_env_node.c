#include "minishell.h"

int	process_global_env_node(t_env_lst *env_lst, char *new_env_var)
{
	t_env_lst	*temp;
	char		*var_name;

	var_name = get_var_name(new_env_var);
	if (var_name == NULL)
		return (write_sys_error("malloc failed"));
	temp = check_if_var_exist(env_lst, var_name);
	free(var_name);
	if (temp == NULL) // READ NOTE BELOW
	{
		temp = env_lst->next;
		env_lst->next = get_global_env_node(new_env_var);
		if (env_lst->next == NULL)
			return (write_sys_error("malloc failed"));
		env_lst->next->next = temp;
	}
	else
	{
		free(temp->value);
		temp->value = get_var_value(new_env_var);
		if (temp->value == NULL)
			return (write_sys_error("malloc failed"));
	}
	return (0);
}

int	process_non_global_env_node(t_env_lst *env_lst, char *new_env_var)
{
	t_env_lst	*temp;
	char		*var_name;

	var_name = get_var_name(new_env_var);
	if (var_name == NULL)
		return (write_sys_error("malloc failed"));
	temp = check_if_var_exist(env_lst, var_name);
	free(var_name);
	if (temp == NULL) // READ NOTE BELOW
	{
		temp = env_lst->next;
		env_lst->next = get_non_global_env_node(new_env_var);
		if (env_lst->next == NULL)
			return (write_sys_error("malloc failed"));
		env_lst->next->next = temp;
	}
	else
	{
		free(temp->value);
		temp->value = get_var_value(new_env_var);
		if (temp->value == NULL)
			return (write_sys_error("malloc failed"));
	}
	return (0);
}

int	process_null_value_env_node(t_env_lst *env_lst, char *new_env_var)
{
	t_env_lst	*temp;
	char		*var_name;

	var_name = get_var_name(new_env_var);
	if (var_name == NULL)
		return (write_sys_error("malloc failed"));
	temp = check_if_var_exist(env_lst, var_name);
	free(var_name);
	if (temp == NULL) // READ NOTE BELOW
	{
		temp = env_lst->next;
		env_lst->next = get_null_value_env_node(new_env_var);
		if (env_lst->next == NULL)
			return (write_sys_error("malloc failed"));
		env_lst->next->next = temp;
	}
	return (0);
}

// NOTE RELATED TO NEW VAR POSITION IN LIST:
// now in my code, the new env_var goes to the second position of env_linked_list. Is this ok? bash seemed to do things quite randomly...
// if you export multiple variables at the same time (multiple args), they go right next to each other
// otherwise, it seems random where they go in the list!