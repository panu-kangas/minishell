/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 08:45:52 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/08 09:17:18 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	str[12];
	int		i;

	i = 0;
	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n == 0)
		ft_putstr_fd("0", fd);
	else if (n < 0)
	{
		ft_putchar_fd ('-', fd);
		n *= -1;
	}
	while (n > 0)
	{
		str[i] = (n % 10) + 48;
		n = n / 10;
		i++;
	}
	i--;
	while (i >= 0)
	{
		ft_putchar_fd(str[i], fd);
		i--;
	}
}
