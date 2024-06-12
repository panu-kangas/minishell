/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_env_var.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:37:44 by pkangas           #+#    #+#             */
/*   Updated: 2024/06/12 15:23:41 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_for_limiter(char *hd_str, char *limiter, int hd_len, int limit_len)
{
	int	i;
	int	j;

	i = hd_len - 1 - limit_len;
	j = 0;
	if (hd_str != NULL && ft_strlen(hd_str) == 1 && hd_str[0] == '\n')
		return (0);
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

int	write_expanded_env_var(t_env *env_lst, char *var_name, int fd, int w_len)
{
	char		*var_value;
	int			var_name_len;

	if (var_name == NULL)
		return (-1);
	else if (w_len > 60000)
		return (0);
	var_name_len = ft_strlen(var_name);
	var_value = expand_env_var(env_lst, var_name);
	if (var_value != NULL)
		write(fd, var_value, ft_strlen(var_value));
	return (var_name_len);
}

int	write_expanded_hd(t_env *env_lst, char *hd_str, char *limiter, int fd)
{
	int		i;
	int		prev_i;
	int		whole_len;
	char	*var_name;

	remove_limiter(hd_str, limiter);
	i = 0;
	whole_len = 0;
	while (hd_str[i] != '\0')
	{
		prev_i = i;
		if (hd_str[i] == '$' && (ft_isalnum(hd_str[i + 1]) == 1 \
		|| hd_str[i + 1] == '_'))
		{
			var_name = find_env_var_name(hd_str, i + 1);
			whole_len += ft_strlen(expand_env_var(env_lst, var_name)) - 1;
			i += write_expanded_env_var(env_lst, var_name, fd, whole_len) + 1;
			free(var_name);
		}
		else
			write(fd, &hd_str[i++], 1);
		if (check_for_hd_error(i, prev_i, &whole_len) == 1)
			return (free_hd_str(hd_str, 1));
	}
	return (free_hd_str(hd_str, 0));
}
