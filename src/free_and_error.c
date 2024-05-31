/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:56:58 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/31 15:14:14 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_free_doubleptr(char **ptr)
{
	int	i;

	i = 0;
	if (ptr != NULL)
	{
		while (ptr[i] != NULL)
			free(ptr[i++]);
	}
	free(ptr);
	return (NULL);
}

void	close_hd_pipes(t_data *data)
{
	t_token	*cur_token;
	t_file	*cur_file;

	cur_token = data->tokens;
	while (cur_token != NULL)
	{
		cur_file = cur_token->files;
		while (cur_file != NULL)
		{
			close_and_init_fd(&cur_file->hd_pipe[0]);
			close_and_init_fd(&cur_file->hd_pipe[1]);
			cur_file = cur_file->next;
		}
		cur_token = cur_token->next;
	}
}

static void	free_token(t_token *token)
{
	t_token	*temp;
	t_file	*tempfile;

	temp = token;
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
		ft_free_doubleptr(temp->args);
	if (temp->com != NULL)
		free (temp->com);
}

int	ft_free_data(t_data *data, int code)
{
	t_token	*temp;

	temp = data->tokens;
	close_hd_pipes(data);
	while (temp != NULL)
	{
		free_token(temp);
		temp = temp->next;
		free(data->tokens);
		data->tokens = temp;
	}
	if (data->input != NULL)
		free(data->input);
	free (data);
	return (code);
}

int	ft_free_parse(t_parse *head, int exit_status)
{
	t_parse	*temp;

	temp = head;
	while (temp != NULL)
	{
		if (temp->str != NULL)
			free(temp->str);
		temp = temp->next;
		free(head);
		head = temp;
	}
	return (exit_status);
}
