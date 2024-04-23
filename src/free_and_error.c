/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:56:58 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/23 15:15:14 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_double(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

int	ft_free_data(t_data *data, int code)
{
	t_token	*temp;
	t_file	*tempfile;

	while (data->tokens != NULL)
	{
		temp = data->tokens;
		while (temp->files != NULL)
		{
			tempfile = temp->files;
			if (tempfile->filename != NULL)
				free (tempfile->filename);
			temp->files = tempfile->next;
			if (tempfile != NULL)
				free (tempfile);
		}
		if (temp->args != NULL)
			ft_free_double(temp->args);
		if (temp->com != NULL)
			free (temp->com);
		data->tokens = temp->next;
		free(temp);
	}
	if (data->input != NULL)
		free(data->input);
	free (data);
	return (code);
}
