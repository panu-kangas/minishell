#include "minishell.h"

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
