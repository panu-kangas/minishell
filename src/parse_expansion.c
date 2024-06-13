/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:20:40 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/13 10:01:01 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_array_len(char **arr)
{
	int	len;

	if (!arr)
		return (0);
	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

static int	expand_token(t_token *current, t_env *env_lst, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (current->com != NULL)
	{
		exit_status = expand_com(current, env_lst, data, exit_status);
		if (exit_status != 0)
			return (exit_status);
	}
	if (current->args != NULL || ft_array_len(current->args) > 0)
	{
		exit_status = expand_args(current, env_lst, data, exit_status);
		if (exit_status != 0)
			return (exit_status);
	}
	if (current->files != NULL)
	{
		exit_status = expand_files(current, env_lst, data, exit_status);
		if (exit_status != 0)
			return (exit_status);
	}
	return (0);
}

int	parse_expansions(t_data *data, t_env *env_lst)
{
	t_token	*temp_token;
	int		exit_status;

	exit_status = 0;
	temp_token = data->tokens;
	while (temp_token != NULL)
	{
		env_lst->cur_token = temp_token;
		exit_status = expand_token(temp_token, env_lst, data);
		if (exit_status != 0)
			return (exit_status);
		temp_token = temp_token->next;
	}
	return (exit_status);
}
