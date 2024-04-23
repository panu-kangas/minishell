/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:20:13 by pkangas           #+#    #+#             */
/*   Updated: 2023/12/01 13:06:18 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static unsigned int	power_hex(int power)
{
	unsigned int	num;

	num = 0;
	if (power == 0)
		return (1);
	while (power > 0)
	{
		if (num == 0)
			num = 16;
		else
			num = num * 16;
		power--;
	}
	return (num);
}

static int	get_len(unsigned int num)
{
	int	hex_len;

	hex_len = 0;
	while (num > 0)
	{
		num = num / 16;
		hex_len++;
	}
	return (hex_len);
}

int	print_hexa(unsigned int num, char c, int count)
{
	int		hex_len;
	int		result;
	char	*hexa;

	if (c == 'x')
		hexa = "0123456789abcdef";
	else
		hexa = "0123456789ABCDEF";
	hex_len = get_len(num);
	if (num == 0)
		hex_len = 1;
	count = count + hex_len;
	while (hex_len > 0)
	{
		result = num / power_hex(hex_len - 1);
		if (write(1, &hexa[result], 1) == -1)
			return (-1);
		num = num - (result * power_hex(hex_len - 1));
		hex_len--;
	}
	return (count);
}
