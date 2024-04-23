
/*int	ft_free_tokens(t_data *data, int code)
{
	t_token *temp;
	t_file *tempfile;
	int i;

	i = 0;
	temp = data->tokens;
	while (temp != NULL)
	{
		tempfile = temp->files;
		while(tempfile != NULL)
		{
			free (tempfile->filename);
			tempfile = tempfile->next;
		}
		if (temp->args != NULL)
		{
			while(temp->args[i] != NULL)
			{
				free(temp->args[i]);
				i++;
			}
		}
		free (temp->com);
		temp = temp->next;
	}
	data->tokens = NULL;
	return(code);
}*/