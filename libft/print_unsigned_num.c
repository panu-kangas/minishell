/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned_num.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:10:55 by pkangas           #+#    #+#             */
/*   Updated: 2023/12/01 12:13:09 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static long int	ft_power(int nb, int power)
{
	long int	x;

	x = nb;
	if (power < 0)
		return (0);
	else if (power == 0)
		return (1);
	else
	{
		while (power > 1)
		{
			x = x * nb;
			power--;
		}
		return (x);
	}
}

static int	get_digits(unsigned int n)
{
	int	digits;

	digits = 0;
	while ((n / ft_power(10, digits)) > 0)
	{
		digits++;
	}
	return (digits);
}

int	print_unsigned_num(unsigned int un, int count)
{
	int				digits;
	char			num;

	if (un == 0)
		count = write (1, "0", 1);
	else
	{
		digits = get_digits(un);
		while (--digits >= 0)
		{
			num = '0' + (un / ft_power(10, digits));
			un = un - (ft_power(10, digits) * (un / ft_power(10, digits)));
			if (write(1, &num, 1) == -1)
				return (-1);
			count = count + 1;
		}
	}
	return (count);
}
