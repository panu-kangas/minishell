/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:17:05 by pkangas           #+#    #+#             */
/*   Updated: 2024/01/23 12:25:01 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

static unsigned long int	power_pointer(int power)
{
	unsigned long int	num;

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

static int	get_len_pointer(unsigned long int num)
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

static int	print_hexa_pointer(unsigned long int num, char c, int count)
{
	int		hex_len;
	int		result;
	char	*hexa;

	if (c == 'x')
		hexa = "0123456789abcdef";
	else
		hexa = "0123456789ABCDEF";
	hex_len = get_len_pointer(num);
	if (num == 0)
		hex_len = 1;
	count = count + hex_len;
	while (hex_len > 0)
	{
		result = num / power_pointer(hex_len - 1);
		if (write(1, &hexa[result], 1) == -1)
			return (-1);
		num = num - (result * power_pointer(hex_len - 1));
		hex_len--;
	}
	return (count);
}

int	print_pointer(void *ptr)
{
	int					count;
	unsigned long int	address;

	count = 0;
	if (ptr == NULL)
	{
		if (write(1, "0x0", 3) == -1)
			return (-1);
		else
			return (3);
	}
	if (write(1, "0x", 2) == -1)
		return (-1);
	count = 2;
	address = (unsigned long int)ptr;
	count = print_hexa_pointer(address, 'x', count);
	return (count);
}
