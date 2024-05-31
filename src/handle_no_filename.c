/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_no_filename.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 07:43:59 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/31 14:40:06 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_no_file(char **tokenarr, int i, int exit_status)
{
	if (check_redir(tokenarr[i]) > 0 && check_redir(tokenarr[i]) \
	< 9 && ft_strlen(tokenarr[i]) < 3)
	{
		if (tokenarr[i + 1] == NULL)
			return (write_syntax_error("newline"));
		else if (tokenarr[i + 1][0] == '>')
			return (write_syntax_error(">"));
		else if (tokenarr[i + 1][0] == '<')
			return (write_syntax_error("<"));
	}
	else if (check_redir(tokenarr[i]) > 4 && check_redir(tokenarr[i]) < 9)
	{
		if (tokenarr[i + 1][2] == '>')
			return (write_syntax_error(">"));
		else if (tokenarr[i + 1][2] == '<')
			return (write_syntax_error("<"));
	}
	return (exit_status);
}
