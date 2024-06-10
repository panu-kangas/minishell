/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:43:26 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/03 13:12:48 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data)
{
	char	*pwd;

	errno = 0;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL && errno == ENOMEM)
		return (write_sys_error("malloc failed"));
	else if (pwd == NULL)
	{
		printf("%s\n", data->current_directory);
		return (0);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

void	ft_env(t_env *env_lst)
{
	while (env_lst != NULL)
	{
		if (env_lst->is_global == 1 && env_lst->value != NULL)
			printf("%s=%s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
}
