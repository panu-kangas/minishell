#include "minishell.h"

int	check_empty_rl_input(t_data *data)
{
	t_token *token;

	token = data->tokens;
	if (token->next != NULL && token->com[0] == '\0')
		return (1);
	else
		return (0);
}