/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:27:34 by pkangas           #+#    #+#             */
/*   Updated: 2023/11/14 10:43:14 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*clear_list(void *content, t_list *head, void (*del)(void *))
{
	del(content);
	ft_lstclear(&head, del);
	return (0);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*temp;
	void	*content;

	if (lst == 0 || f == 0 || del == 0)
		return (0);
	content = f(lst->content);
	head = ft_lstnew(content);
	if (head == 0)
	{
		del(content);
		return (0);
	}
	temp = head;
	while (lst->next != 0)
	{
		lst = lst->next;
		content = f(lst->content);
		temp->next = ft_lstnew(content);
		if (temp->next == 0)
			return (clear_list(content, head, del));
		temp = temp->next;
	}
	temp->next = 0;
	return (head);
}
