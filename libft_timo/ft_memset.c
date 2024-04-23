/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:40 by tsaari            #+#    #+#             */
/*   Updated: 2023/11/03 14:07:35 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned long	i;
	unsigned char	*copy;

	i = 0;
	copy = str;
	while (i < n)
	{
		copy[i] = c;
		i++;
	}
	return (str);
}
