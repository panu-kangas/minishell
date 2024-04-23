/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:49:34 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:07:00 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
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
			found = (char *)&s[i];
		i++;
	}
	if (s[i] == a)
		found = (char *)&s[i];
	return (found);
}
