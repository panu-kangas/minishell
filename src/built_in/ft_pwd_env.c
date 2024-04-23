#include "minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (write_sys_error("malloc failed"));
	ft_printf("%s\n", pwd);
	free(pwd);
}

void	ft_env(t_env_lst *env_lst)
{
	while (env_lst != NULL)
	{
		if (env_lst->is_global == 1 && env_lst->value != NULL)
			ft_printf("%s=%s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
}