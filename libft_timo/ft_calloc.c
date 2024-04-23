/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 17:25:11 by tsaari            #+#    #+#             */
/*   Updated: 2024/03/20 16:24:54 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	if (count && size && count > (UINT_MAX / size))
		return (0);
	ret = (malloc(count * size));
	if (!ret)
		return (0);
	ft_bzero (ret, count * size);
	return (ret);
}
