/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_empty_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:34:57 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/07 11:04:01 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_lstsize(t_env *lst)
{
	int	size;

	size = 0;
	while (lst != NULL)
	{
		if (lst->is_global == 1 && ft_strncmp(lst->name, "_", 2) != 0)
			size++;
		lst = lst->next;
	}
	return (size);
}

char	**copy_env_var(t_env *env_lst, char **alph_order)
{
	int	i;

	alph_order = malloc((env_lstsize(env_lst) + 1) * sizeof(char *));
	if (alph_order == NULL)
		return (NULL);
	i = 0;
	while (env_lst != NULL)
	{
		if (env_lst->is_global == 1 && ft_strncmp(env_lst->name, "_", 2) != 0)
		{
			alph_order[i] = ft_strdup(env_lst->name);
			if (alph_order[i] == NULL)
				return (NULL);
			i++;
		}
		env_lst = env_lst->next;
	}
	alph_order[i] = NULL;
	return (alph_order);
}

char	**get_env_var_alph_order(t_env *env_lst, char **alph_order)
{
	char	*temp;
	int		lstsize;
	int		i;
	int		j;

	alph_order = copy_env_var(env_lst, alph_order);
	if (alph_order == NULL)
		return (NULL);
	lstsize = env_lstsize(env_lst);
	i = -1;
	while (++i < (lstsize - 1))
	{
		j = -1;
		while (++j < (lstsize - 1))
		{
			if (ft_strncmp(alph_order[j], \
			alph_order[j + 1], ft_strlen(alph_order[j])) > 0)
			{
				temp = alph_order[j];
				alph_order[j] = alph_order[j + 1];
				alph_order[j + 1] = temp;
			}
		}
	}
	return (alph_order);
}

int	ft_empty_export(t_env *env_lst)
{
	char	**alph_order;
	int		i;
	t_env	*temp;

	alph_order = NULL;
	alph_order = get_env_var_alph_order(env_lst, alph_order);
	if (alph_order == NULL)
	{
		ft_free_doubleptr(alph_order);
		return (1);
	}
	i = -1;
	while (alph_order[++i] != NULL)
	{
		temp = check_if_var_exist(env_lst, alph_order[i]);
		if (temp->value == NULL)
			printf("declare -x %s\n", alph_order[i]);
		else
		{
			printf("declare -x %s=\"%s\"\n", \
			alph_order[i], expand_env_var(env_lst, alph_order[i]));
		}
	}
	ft_free_doubleptr(alph_order);
	return (0);
}
