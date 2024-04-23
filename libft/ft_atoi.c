/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:10:47 by tsaari            #+#    #+#             */
/*   Updated: 2024/02/12 14:05:01 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	converttoint(const char *str, int i, int neg)
{
	long	ret;

	ret = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		ret = ret * 10 + str[i] - 48;
		if (ret < 0 && neg == 1)
			return ((int)LONG_MAX);
		else if (ret < 0 && neg == -1)
			return ((int)LONG_MIN);
		i++;
	}
	return (ret);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		neg;

	neg = 1;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	else if (!(str[i] >= 48 && str[i] <= 57))
		return (0);
	return (neg * converttoint(str, i, neg));
}
