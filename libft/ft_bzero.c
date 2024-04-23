/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 10:43:55 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:05:26 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	unsigned char	*char_s;

	i = 0;
	char_s = s;
	if (n > 0)
	{
		while (i < n)
		{
			char_s[i] = '\0';
			i++;
		}
	}
}
