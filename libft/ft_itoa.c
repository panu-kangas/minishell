/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:25:51 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:11:17 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	set_number(char *number, int n, int num_size)
{
	long int	j;

	j = n;
	if (j < 0)
		j = j * -1;
	while (num_size >= 0)
	{
		number[num_size] = '0' + j % 10;
		j = j / 10;
		num_size--;
		if (num_size == 0 && n < 0)
		{
			number[0] = '-';
			num_size--;
		}
	}
}

char	*ft_itoa(int n)
{
	int			num_size;
	long int	temp;
	char		*number;

	num_size = 0;
	temp = n;
	if (temp < 0)
	{
		temp = temp * -1;
		num_size++;
	}
	if (n == 0)
		num_size = 1;
	while (temp > 0)
	{
		temp = temp / 10;
		num_size++;
	}
	number = (char *) malloc(num_size + 1);
	if (number == 0)
		return (0);
	number[num_size--] = '\0';
	set_number(number, n, num_size);
	return (number);
}
