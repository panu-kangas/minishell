/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_num.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:15:56 by pkangas           #+#    #+#             */
/*   Updated: 2023/12/01 12:14:37 by pkangas          ###   ########.fr       */
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

static int	get_digits(int n)
{
	int	digits;

	digits = 0;
	while ((n / ft_power(10, digits)) > 0)
	{
		digits++;
	}
	return (digits);
}

int	print_num(int n, int count)
{
	int		digits;
	char	num;

	if (n == 0)
		count = write (1, "0", 1);
	else if (n == -2147483648)
		count = write (1, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			count = write (1, "-", 1);
			n = (n * -1);
		}
		digits = get_digits(n);
		while (--digits >= 0 && count != -1)
		{
			num = '0' + (n / ft_power(10, digits));
			n = n - (ft_power(10, digits) * (n / ft_power(10, digits)));
			if (write(1, &num, 1) == -1)
				return (-1);
			count = count + 1;
		}
	}
	return (count);
}
