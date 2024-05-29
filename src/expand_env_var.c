#include "minishell.h"

// In calling function: if expand_env_var returns NULL, skip the argument totally!!
// for example:
// bash-3.2$ echo $NON_EXISTING hi hello
// hi hello


// The case of $? needs it's own separate handling !!!


char	*expand_env_var(t_env_lst *env_lst, char *var_name)
{
	t_env_lst	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char	**expand_env_var_array(t_env_lst *env_lst, char *var_name)
{
	t_env_lst	*temp;

	temp = env_lst;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, var_name, ft_strlen(var_name) + 1) == 0)
			return (ft_split(temp->value, ' '));
		temp = temp->next;
	}
	return (NULL);
}
