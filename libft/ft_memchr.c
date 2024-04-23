/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:54:56 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/09 11:50:08 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned int			i;
	unsigned char			a;
	unsigned char			*found;
	const unsigned char		*char_s;

	i = 0;
	found = 0;
	a = (unsigned char)c;
	char_s = s;
	while (i < n)
	{
		if (char_s[i] == a)
		{
			found = (unsigned char *)&char_s[i];
			return (found);
		}
		i++;
	}
	return (found);
}
