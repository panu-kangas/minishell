/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:31:38 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/08 14:14:10 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;
	t_list	*node;

	if (lst != 0 && del != 0)
	{
		node = *lst;
		while (node != 0)
		{
			temp = node->next;
			del(node->content);
			free(node);
			node = temp;
		}
	}
	*lst = 0;
}
