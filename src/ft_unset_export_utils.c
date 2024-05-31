/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:32:41 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 12:32:43 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_check_valid_input(char *var_name)
{
	int	i;

	if (var_name != NULL && ft_isalpha(var_name[0]) == 0 && var_name[0] != '_')
		return (write_exp_error("unset", var_name));
	i = 1;
	while (var_name != NULL && var_name[i] != '\0')
	{
		if (ft_isalnum(var_name[i]) == 0 && var_name[i] != '_')
			return (write_exp_error("unset", var_name));
		i++;
	}
	return (0);
}

int	export_check_valid_input(char *new_env_var)
{
	int	i;

	if (new_env_var != NULL && ft_isalpha(new_env_var[0]) == 0 \
	&& new_env_var[0] != '_')
		return (write_exp_error("export", new_env_var));
	i = 1;
	while (new_env_var != NULL && new_env_var[i] != '=' \
	&& new_env_var[i] != '\0')
	{
		if (ft_isalnum(new_env_var[i]) == 0 && new_env_var[i] != '_')
			return (write_exp_error("export", new_env_var));
		i++;
	}
	return (0);
}
