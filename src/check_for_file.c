#include "minishell.h"

int	check_for_outfile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == -1)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	check_for_infile(t_token *cur_token)
{
	t_file	*temp;

	temp = cur_token->files;
	while (temp != NULL)
	{
		if (temp->is_infile == 1)
			return (1);
		temp = temp->next;
	}
	return (0);
}