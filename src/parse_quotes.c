/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 10:27:14 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/20 15:44:12 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int remove_quotes_com(t_data *data, char c, char d)
{
	int i;
	int j;
	int ctr;
	char *temp;

	temp = data->tokens->com;
	i = 0;
	j = 0;
	ctr = 0;
	int len = ft_strlen(temp);
	while(i < len)
	{
		if (temp[i] == c)
		{
			while(temp[i] == c)
				i++;
			if (temp[i] == d && temp [i + 1] != 0)
			{
				ctr = count_quotes((temp + i), d, c);
			}
		}
		if (ctr % 2 != 0)
		{
			ft_putendl_fd("syntax error, invalid amount of quotes", 2);
			return(258);
		}
		if (temp[i] != c)
			temp[j++] = temp[i];
		i++;
	}
	temp[j] = 0;
	data->tokens->com = ft_strdup(temp);
	free (temp);
	return (0);
}

int remove_quotes_args(t_data *data, int k, char c, char d)
{
	int i;
	int j;
	int ctr;
	char *temp;

	temp = data->tokens->args[k];
	i = 0;
	j = 0;
	ctr = 0;
	int len = ft_strlen(temp);
	while(i < len)
	{
		if (temp[i] == c)
		{
			while(temp[i] == c)
				i++;
			if (temp[i] == d && temp [i + 1] != 0)
			{
				ctr = count_quotes((temp + i), d, c);
			}
		}
		if (ctr % 2 != 0)
		{
			ft_putendl_fd("syntax error, invalid amount of quotes", 2);
			return(258);
		}
		if (temp[i] != c)
			temp[j++] = temp[i];
		i++;
	}
	temp[j] = 0;
	data->tokens->args[k] = ft_strdup(temp);
	free (temp);
	return (0);
}

int check_quotes_args(t_data *data, int i, char c, char d)
{
	int exit_status;

	exit_status = 0;
	if (count_quotes(data->tokens->args[i], c, '\0') % 2 != 0)
	{
		ft_putendl_fd("syntax error, invalid amount of quotes", 2);
		return (258);
	}
	else
	{
		exit_status = remove_quotes_args(data, i, c, d);
		if (!data->tokens->args[i])
			return (write_sys_error("malloc failed"));
	}
	return (exit_status);
}

int check_quotes_com(t_data *data, char c, char d)
{
	int exit_status;

	exit_status = 0;
	if (count_quotes(data->tokens->com, c, '\0') % 2 != 0)
	{
		ft_putendl_fd("syntax error, invalid amount of quotes", 2);
		return (258);
	}
	else
	{
		exit_status = remove_quotes_com(data, c, d);
		if (!data->tokens->com)
			return (write_sys_error("malloc failed"));
	}
	return (exit_status);
}

int parse_args_quotes(t_data *data, int exit_status)
{
	int i;
	int j;

	i = 0;
	while (data->tokens->args != NULL && data->tokens->args[i] != 0)
	{
		j = 0;
		while(data->tokens->args[i][j] != 0)
		{
			if (data->tokens->args[i][j] == '\'')
			{
				exit_status = check_quotes_args(data, i, '\'', '\"');
				break ;
			}
			else if (data->tokens->args[i][j] == '\"')
			{
				exit_status = check_quotes_args(data, i, '\"', '\'');
				break ;
			}
			j++;
		}
		i++;
	}
	return (exit_status);
}

int parse_com_quotes(t_data *data, int exit_status)
{
	int j;

	j = 0;
	while(data->tokens->com != NULL && data->tokens->com[j] != 0)
	{
		if (data->tokens->com[j] == '\'')
		{
			exit_status = check_quotes_com(data, '\'', '\"');
			break ;
		}
		else if (data->tokens->com[j] == '\"')
		{
			exit_status = check_quotes_com(data, '\"', '\'');
			break ;
		}
		j++;
	}
	return (exit_status);
}

int parse_out_quotes(t_data *data, int exit_status)
{
	exit_status = parse_args_quotes(data, exit_status);
	if (exit_status != 0)
		return (exit_status);
	exit_status = parse_com_quotes(data, exit_status);
	return (exit_status);
}