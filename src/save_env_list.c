#include "minishell.h"

char	*get_var_value(char *environ_var)
{
	char	*var_value;
	int		i;
	int		j;
	int		len;
	int		value_start_i;

	i = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		i++;
	if (environ_var[i] != '\0')
		i++;
	value_start_i = i;
	len = 0;
	while (environ_var[i++] != '\0')
		len++;
	var_value = malloc(len + 1);
	if (var_value == NULL)
		return (NULL);
	i = value_start_i;
	j = 0;
	while (environ_var[i] != '\0')
		var_value[j++] = environ_var[i++];
	var_value[j] = '\0';
	return (var_value);
}

char	*get_var_name(char *environ_var)
{
	char	*var_name;
	int		i;
	int		j;
	int		len;

	i = 0;
	len = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
	{
		i++;
		len++;
	}
	var_name = malloc(len + 1);
	if (var_name == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (environ_var[i] != '=' && environ_var[i] != '\0')
		var_name[j++] = environ_var[i++];
	var_name[j] = '\0';
	return (var_name);
}

int	process_shlvl(t_env_lst *env_lst)
{
	t_env_lst	*shlvl;
	int			value;

	shlvl = check_if_var_exist(env_lst, "SHLVL");
	if (shlvl == NULL)
	{
		shlvl = get_global_env_node("SHLVL=1");
		if (shlvl == NULL)
			return (1);
		shlvl->next = env_lst->next;
		env_lst->next = shlvl;
	}
	else
	{
		if (shlvl->value == NULL || shlvl->value[0] == '\0')
			value = 0;
		else
			value = ft_atoi(shlvl->value);
		value++;
		if (value < 0)
			value = 0;
		else if (value > 1000)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putnbr_fd(value, 2);
			ft_putendl_fd(") too high, resetting to 1", 2);
			value = 1;
		}
		free(shlvl->value);
		if (value == 1000)
			shlvl->value = ft_strdup("");
		else
			shlvl->value = ft_itoa(value);
		if (shlvl->value == NULL)
			return (1);
	}
	return (0);
}

int	make_pwd_variables(t_env_lst *env_lst)
{
	t_env_lst	*temp;
	char		*cur_dir;
	char		*pwd;

	temp = env_lst;
	while (temp->next != NULL)
		temp = temp->next;
	if (check_if_var_exist(env_lst, "OLDPWD") == NULL)
	{
		temp->next = get_null_value_env_node("OLDPWD");
		if (temp->next == NULL)
			return (1);
		temp = temp->next;
	}
	if (check_if_var_exist(env_lst, "PWD") == NULL)
	{
		cur_dir = getcwd(NULL, 0);
		if (cur_dir == NULL)
			return (1);
		pwd = ft_strjoin("PWD=", cur_dir);
		free(cur_dir);
		if (pwd == NULL)
			return (1);
		temp->next = get_global_env_node(pwd);
		free(pwd);
		if (temp->next == NULL)
			return (1);
	}
	return (0);
}

t_env_lst	*save_env_list(char **environ)
{
	int			i;
	t_env_lst	*env_lst;
	t_env_lst	*temp;

	env_lst = NULL;
	if (environ != NULL && environ[0] != NULL)
		env_lst = get_global_env_node(environ[0]);
	else
		env_lst = get_null_value_env_node("OLDPWD");
	if (env_lst == NULL)
		return (NULL);
	temp = env_lst;
	i = 1;
	while (environ != NULL && environ[0] != NULL && environ[i] != NULL)
	{
		temp->next = get_global_env_node(environ[i++]);
		temp = temp->next;
		if (temp == NULL)
			return (NULL);
	}
	if (make_pwd_variables(env_lst) == 1)
		return (NULL);
	if (process_shlvl(env_lst) == 1)
		return (NULL);
	return (env_lst);
}
