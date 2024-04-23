/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:11:29 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:12:33 by pkangas          ###   ########.fr       */
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
	long int	digits;

	digits = 0;
	while ((n / ft_power(10, digits)) > 0)
	{
		digits++;
	}
	return (digits);
}

void	ft_putnbr_fd(int n, int fd)
{
	int		digits;
	char	num;

	if (n == 0)
		write (fd, "0", 1);
	else if (n == -2147483648)
		write (fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			write (fd, "-", 1);
			n = (n * -1);
		}
		digits = get_digits(n) - 1;
		while (digits >= 0)
		{
			num = '0' + (n / ft_power(10, digits));
			n = n - (ft_power(10, digits) * (n / ft_power(10, digits)));
			write(fd, &num, 1);
			digits--;
		}
	}
}
