/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:22:17 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/29 13:00:39 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	print_char(int i)
{
	char	c;

	c = (char) i;
	if (write(1, &c, 1) == -1)
		return (-1);
	else
		return (1);
}

int	print_str(char *str)
{
	int	count;
	int	i;

	count = 0;
	if (str == NULL)
	{
		count = write(1, "(null)", 6);
		return (count);
	}
	i = 0;
	while (str[i] != '\0')
	{
		if (write(1, &str[i], 1) == -1)
			return (-1);
		else
			count++;
		i++;
	}
	return (count);
}
