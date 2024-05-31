/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_env_var.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:37:44 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 10:37:45 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_for_limiter(char *hd_str, char *limiter, int hd_len, int limit_len)
{
	int	i;
	int	j;

	i = hd_len - 1 - limit_len;
	j = 0;
	if (limit_len == 0 && hd_str[i] == '\n' && hd_str[i - 1] == '\n')
		return (1);
	else if (limit_len == 0)
		return (0);
	if (i > 0 && hd_str[i - 1] != '\n')
		return (0);
	while (limiter[j] != '\0')
	{
		if (limiter[j++] != hd_str[i++])
			break ;
	}
	if (limiter[j] == '\0' && hd_str[i] == '\n')
		return (1);
	else
		return (0);
}

void	remove_limiter(char *hd_str, char *limiter)
{
	int	hd_len;
	int	limit_len;
	int	is_limiter;

	hd_len = ft_strlen(hd_str);
	limit_len = ft_strlen(limiter);
	is_limiter = search_for_limiter(hd_str, limiter, hd_len, limit_len);
	if (limit_len == 0 && is_limiter == 1)
		hd_str[hd_len - 1] = '\0';
	else if (is_limiter == 1)
		hd_str[hd_len - limit_len - 1] = '\0';
}

char	*find_env_var_name(char *hd_str, int i)
{
	int	orig_i;
	int	var_name_len;

	orig_i = i;
	var_name_len = 0;
	if (ft_isdigit(hd_str[i]) == 1)
		return (ft_substr(hd_str, i, 1));
	while (hd_str[i] != '\0')
	{
		if (ft_isalnum(hd_str[i]) == 0 && hd_str[i] != '_')
			break ;
		i++;
		var_name_len++;
	}
	i = orig_i;
	return (ft_substr(hd_str, i, var_name_len));
}

int	write_expanded_env_var(t_env_lst *env_lst, char *hd_str, int i, int fd)
{
	char	*var_name;
	char	*var_value;
	int		var_name_len;

	var_name = find_env_var_name(hd_str, i);
	if (var_name == NULL)
		return (-1);
	var_name_len = ft_strlen(var_name);
	var_value = expand_env_var(env_lst, var_name);
	if (var_name[0] == '0')
		write(fd, "./minishell", 11);
	free(var_name);
	if (var_value != NULL)
		write(fd, var_value, ft_strlen(var_value));
	return (var_name_len);
}

int	write_expanded_hd(t_env_lst *env_lst, char *hd_str, char *limiter, int fd)
{
	int	i;
	int	prev_i;

	remove_limiter(hd_str, limiter);
	i = 0;
	while (hd_str[i] != '\0')
	{
		prev_i = i;
		if (hd_str[i] == '$' && (ft_isalnum(hd_str[i + 1]) == 1 \
		|| hd_str[i + 1] == '_'))
			i += write_expanded_env_var(env_lst, hd_str, i + 1, fd) + 1;
		else
			write(fd, &hd_str[i++], 1);
		if (prev_i == i)
			return (write_sys_error("malloc failed"));
	}
	return (0);
}
