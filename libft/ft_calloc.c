/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 14:54:13 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/20 14:26:56 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	unsigned int	i;
	size_t			max_nbr;
	char			*p;

	max_nbr = 0 - 1;
	if (count != 0 && (max_nbr / count) < size)
		return (0);
	p = malloc(count * size);
	if (p == 0)
		return (0);
	i = 0;
	while (i < (count * size))
	{
		p[i] = '\0';
		i++;
	}
	return (p);
}
