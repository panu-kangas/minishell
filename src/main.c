/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:51:38 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/30 11:00:18 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{

	if (!envp)
	{
		//errorhandling
	}
	if (argc != 1 || argv == NULL) // this needs to be checked
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
