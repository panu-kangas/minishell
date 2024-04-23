#include "minishell.h"

t_env_lst	*get_global_env_node(char *environ_var)
{
	t_env_lst	*new_node;

	new_node = malloc(sizeof(t_env_lst));
	if (new_node == NULL)
		return (MALLOC_ERR);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (MALLOC_ERR);
	}
	new_node->value = get_var_value(environ_var);
	if (new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node);
		return (MALLOC_ERR);
	}
	new_node->next = NULL;
	new_node->is_global = 1;
	return (new_node);
}

t_env_lst	*get_non_global_env_node(char *environ_var)
{
	t_env_lst	*new_node;

	new_node = malloc(sizeof(t_env_lst));
	if (new_node == NULL)
		return (MALLOC_ERR);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (MALLOC_ERR);
	}
	new_node->value = get_var_value(environ_var);
	if (new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node);
		return (MALLOC_ERR);
	}
	new_node->next = NULL;
	new_node->is_global = 0;
	return (new_node);
}

t_env_lst	*get_null_value_env_node(char *environ_var)
{
	t_env_lst	*new_node;

	new_node = malloc(sizeof(t_env_lst));
	if (new_node == NULL)
		return (MALLOC_ERR);
	new_node->name = get_var_name(environ_var);
	if (new_node->name == NULL)
	{
		free(new_node);
		return (MALLOC_ERR);
	}
	new_node->value = NULL;
	new_node->next = NULL;
	new_node->is_global = 1;
	return (new_node);
}
