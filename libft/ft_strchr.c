/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:47:07 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:06:50 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	a;
	char	*found;

	i = 0;
	a = (char)c;
	found = 0;
	while (s[i] != '\0')
	{
		if (s[i] == a)
		{
			found = (char *)&s[i];
			return (found);
		}
		i++;
	}
	if (s[i] == a)
		found = (char *)&s[i];
	return (found);
}
