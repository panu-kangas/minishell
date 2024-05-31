/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_name_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:17:27 by pkangas           #+#    #+#             */
/*   Updated: 2024/05/31 16:17:28 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *environ_var)
{
	char	*var_value;
	int		i;
	int		j;
	int		len;
	int		value_start_i;

	i = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		i++;
	if (environ_var[i] != '\0')
		i++;
	value_start_i = i;
	len = 0;
	while (environ_var[i++] != '\0')
		len++;
	var_value = malloc(len + 1);
	if (var_value == NULL)
		return (NULL);
	i = value_start_i;
	j = 0;
	while (environ_var[i] != '\0')
		var_value[j++] = environ_var[i++];
	var_value[j] = '\0';
	return (var_value);
}

char	*get_var_name(char *environ_var)
{
	char	*var_name;
	int		i;
	int		j;
	int		len;

	i = 0;
	len = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
	{
		i++;
		len++;
	}
	var_name = malloc(len + 1);
	if (var_name == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		var_name[j++] = environ_var[i++];
	var_name[j] = '\0';
	return (var_name);
}
