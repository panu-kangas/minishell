#include "minishell.h"

void	ft_free_doubleptr(char **ptr)
{
	int	i;

	i = 0;
	if (ptr != NULL)
	{
		while (ptr[i] != NULL)
			free(ptr[i++]);
	}
	free(ptr);
}

int	env_lstsize(t_env_lst *lst)
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

char	**copy_env_var(t_env_lst *env_lst, char **alph_order)
{
	int	i;

	alph_order = malloc((env_lstsize(env_lst) + 1) * sizeof(char *));
	if (alph_order == NULL)
		return (MALLOC_ERR);
	i = 0;
	while (env_lst != NULL)
	{
		if (env_lst->is_global == 1)
		{
			alph_order[i] = ft_strdup(env_lst->name);
			if (alph_order[i] == NULL)
				return (MALLOC_ERR);
			i++;
		}
		env_lst = env_lst->next;
	}
	alph_order[i] = NULL;
	return (alph_order);
}

char	**get_env_var_alph_order(t_env_lst *env_lst, char **alph_order)
{
	char	*temp;
	int		lstsize;
	int		i;
	int		j;

	alph_order = copy_env_var(env_lst, alph_order);
	if (alph_order == NULL)
		return (MALLOC_ERR);
	lstsize = env_lstsize(env_lst);
	i = -1;
	while (++i < (lstsize - 1))
	{
		j = -1;
		while (++j < (lstsize - 1))
		{
			if (ft_strncmp(alph_order[j], alph_order[j + 1], ft_strlen(alph_order[j])) > 0)
			{
				temp = alph_order[j];
				alph_order[j] = alph_order[j + 1];
				alph_order[j + 1] = temp;
			}
		}
	}
	return (alph_order);
}

// How to handle possibly empty env_lst ?? A check again in the beginning (if env_lst->name == NULL)

int	ft_empty_export(t_env_lst *env_lst)
{
	char		**alph_order;
	int			i;
	t_env_lst	*temp;
	
	alph_order = NULL;
	alph_order = get_env_var_alph_order(env_lst, alph_order);
	if (alph_order == NULL)
	{
		ft_free_doubleptr(alph_order);
		return (MALLOC_ERR);
	}
	i = 0;
	while (alph_order[i] != NULL)
	{
		temp = check_if_var_exist(env_lst, alph_order[i]);
		if (temp->value == NULL)
			ft_printf("declare -x %s\n", alph_order[i]);
		else
		{
			ft_printf("declare -x %s=\"%s\"\n", \
			alph_order[i], expand_env_var(env_lst, alph_order[i]));
		}
		i++;
	}
	ft_free_doubleptr(alph_order);
	return (0);
}
