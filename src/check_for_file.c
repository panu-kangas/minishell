/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:10:47 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/17 13:10:28 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_outfile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	check_for_infile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == 1)
			return (1);
		temp = temp->next;
	}
	return (0);
}

char	*add_slash_to_first_path(char *path)
{
	char	*temp;

	temp = ft_strjoin("/", path);
	free(path);
	if (temp == NULL)
		return (NULL);
	return (temp);
}

int	check_amb_status(t_file *file, char *fname)
{
	int	i;

	i = 0;
	while (fname[i] != '\0')
	{
		if (fname[i] == '$' && (ft_isalnum(fname[i + 1]) == 1 \
		|| fname[i + 1] == '_') && file->is_amb == 1)
			return (1);
		i++;
	}
	return (0);
}

int	analyze_e_st(int e_st, char **t_p_str, char **temp)
{
	if (e_st == 1)
		return (write_sys_error("malloc error"));
	else if (e_st == -1)
	{
		free(*t_p_str);
		*t_p_str = *temp;
		return (0);
	}
	return (0);
}
