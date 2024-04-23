/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:51:38 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/23 15:35:20 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{

	if (!envp)
	{
		//errorhandling
	}
	if (argc != 1)
	{
		//errorhandling
		return (0);
	}
	else
	{
		return (parsing());
	}
	return(0);
}
