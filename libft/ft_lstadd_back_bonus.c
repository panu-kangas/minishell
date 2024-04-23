/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:55:27 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:13:08 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*ptr;

	if (lst != 0 && new != 0)
	{
		if (*lst == 0)
			*lst = new;
		else
		{
			ptr = *lst;
			while (ptr->next != 0)
				ptr = ptr->next;
			ptr->next = new;
		}
	}
}
