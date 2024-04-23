/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:21:29 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/14 12:13:19 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

static int	check_max(long int finalnum, int sign, const char *str, int i)
{
	if (finalnum >= (LONG_MAX / 10) && sign == 1)
	{
		if (str[i] <= '7' && finalnum == (LONG_MAX / 10))
			return (1);
		else
			return (-1);
	}
	if (finalnum >= (LONG_MAX / 10) && sign == -1)
	{
		if (str[i] <= '8' && finalnum == (LONG_MAX / 10))
			return (1);
		else
			return (0);
	}
	return (1);
}

static int	check_space(const char *str, int i)
{
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	long int		finalnum;

	i = 0;
	finalnum = 0;
	sign = 1;
	i = check_space(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check_max(finalnum, sign, str, i) == -1)
			return (-1);
		if (check_max(finalnum, sign, str, i) == 0)
			return (0);
		finalnum = finalnum * 10 + str[i] - '0';
		i++;
	}
	return (sign * finalnum);
}
