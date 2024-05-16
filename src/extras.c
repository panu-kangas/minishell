/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:06:48 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/15 13:32:42 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void ft_lstiter_parse(t_parse *lst, void (*f)(t_parse *))
{
	if (!lst && !f)
		return ;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}

void printparse(t_parse *node)
{
	printf("node  = %s, %d\n", node->str, node->isexpand);
}


void	printnode(t_token *token)
{
	int i;

	i = 0;
	printf("\nTOKEN\n\n");
	if (token->com != NULL)
		printf("command name: %s\n", token->com);
	printf("command args: ");
	if (token->args != NULL)
	{
		while (token->args[i] != 0)
		{
			printf("%s", token->args[i]);
			if (token->args[i + 1] != 0)
				printf(" - ");
			i++;
		}
	}
	printf("\n");
}

void	printfile(t_file *node)
{
//	int	i;

	if (!node)
		return;
//	i = 0;
	if (node->filename != NULL)
		printf("Filename: %s - ", node->filename);
	if (node->is_append == 1)
		printf ("append (1) - ");
	else
		printf ("not_append (-1) - ");
	if (node->is_infile == 1)
		printf ("infile (1)");
	else
		printf ("outfile (-1)");
	printf ("\n");
}

void	ft_lstiter2_ms(t_file *files, void (*f)(t_file *))
{
	t_file	*temp;

	if (!files && !f)
		return ;
	temp = files;
	while (temp != NULL)
	{
		f(temp);
		temp = temp->next;
	}
}

void	ft_lstiter_ms(t_token *tokens, void (*f)(t_token *))
{
	t_token	*temp;

	temp = tokens;
	if (!tokens || !f)
		return ;
	while (temp != NULL)
	{
		f(temp);
		ft_lstiter2_ms(temp->files, printfile);
		temp = temp->next;
	}
}