/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:56:58 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/30 07:32:17 by tsaari           ###   ########.fr       */
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

int	ft_free_data(t_data *data, int code)
{
	t_token	*temp;
	t_file	*tempfile;

	close_hd_pipes(data);
	while (data->tokens != NULL)
	{
		temp = data->tokens;
		while (temp->files != NULL)
		{
			tempfile = temp->files;
			free (tempfile->filename);
			temp->files = tempfile->next;
			free (tempfile);
		}
		if (temp->args != NULL)
			ft_free_double(temp->args);
		free (temp->com);
		data->tokens = temp->next;
		free(temp);
	}
	if (data->input != NULL)
		free(data->input);
	free (data);
	return (code);
}

int	ft_free_parse(t_parse *head, int exit_status)
{
	t_parse *temp;

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
