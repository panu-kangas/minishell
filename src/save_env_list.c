#include "minishell.h"

char	*get_var_value(char *environ_var)
{
	char	*var_value;
	int		i;
	int		j;
	int		len;
	int		value_start_i;

	i = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		i++;
	if (environ_var[i] != '\0')
		i++;
	value_start_i = i;
	len = 0;
	while (environ_var[i++] != '\0')
		len++;
	var_value = malloc(len + 1);
	if (var_value == NULL)
		return (MALLOC_ERR);
	i = value_start_i;
	j = 0;
	while (environ_var[i] != '\0')
		var_value[j++] = environ_var[i++];
	var_value[j] = '\0';
	return (var_value);
}

char	*get_var_name(char *environ_var)
{
	char	*var_name;
	int		i;
	int		j;
	int		len;

	i = 0;
	len = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
	{
		i++;
		len++;
	}
	var_name = malloc(len + 1);
	if (var_name == NULL)
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		var_name[j++] = environ_var[i++];
	var_name[j] = '\0';
	return (var_name);
}

t_env_lst	*save_env_list(char **environ)
{
	int			i;
	t_env_lst	*env_lst;
	t_env_lst	*temp;

	// IF environ == NULL
	// env_lst = make_empty_node(), which allocates a node but sets all values to NULL
	// Later in the program we must always check if env_lst->name = NULL
	env_lst = NULL;
	if (environ != NULL && environ[0] != NULL)
		env_lst = get_global_env_node(environ[0]);
	if (env_lst == NULL)
		return (MALLOC_ERR);
	temp = env_lst;
	i = 1;
	while (environ != NULL && environ[i] != NULL)
	{
		temp->next = get_global_env_node(environ[i++]);
		temp = temp->next;
		if (temp == NULL)
			return (MALLOC_ERR);
	}
	if (check_if_var_exist(env_lst, "OLDPWD") == NULL) // DO THIS SAME TO PWD!! We definitely need it
	{
		temp->next = get_null_value_env_node("OLDPWD");
		if (temp->next == NULL)
			return (MALLOC_ERR);
	}
	return (env_lst);
}
