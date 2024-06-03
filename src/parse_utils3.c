/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 20:10:01 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 09:56:46 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_only_spaces(t_data *data)
{
	int	i;

	i = 0;
	while (data->input[i] != 0)
	{
		if (data->input[i] != '\t' || data->input[i] != '\n' \
		|| data->input[i] != '\v' || data->input[i] != '\f' \
		|| data->input[i] != '\r' || data->input[i] != ' ')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

t_file	*add_file(char *str, int is_append, int is_infile)
{
	t_file	*new;

	new = (t_file *)malloc (sizeof(t_file));
	if (!new)
		return (NULL);
	init_file(new);
	if (!str)
	{
		new->no_filename = -1;
		return (NULL);
	}
	new->filename = ft_strdup(str);
	if (!new->filename)
	{
		free (new);
		return (NULL);
	}
	new->is_append = is_append;
	new->is_infile = is_infile;
	new->next = NULL;
	return (new);
}
