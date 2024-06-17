/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:07:27 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/17 12:50:03 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_substr(char *str, t_env *env_lst, int arg_nbr)
{
	char	*temp;

	if (!str || !env_lst)
		return (NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
	{
		env_lst->cur_token->arg_is_expanded_empty = arg_nbr;
		temp = ft_strdup("");
	}
	free(str);
	return (temp);
}

static char	*expand_substr_file(char *str, t_env *env_lst, int put_d)
{
	char	*temp;

	if (!str || !env_lst)
		return (NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
	{
		if (put_d != 0)
			temp = ft_strjoin("$", str);
		else
			temp = ft_strdup("");
	}
	free(str);
	return (temp);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*final_str;

	final_str = NULL;
	if (s1 != NULL && s2 != NULL)
	{
		i = 0;
		j = 0;
		final_str = (char *) malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
		if (final_str == 0)
			return (NULL);
		while (s1[i] != '\0')
			final_str[j++] = s1[i++];
		i = 0;
		while (s2[i] != '\0')
			final_str[j++] = s2[i++];
		final_str[j] = '\0';
	}
	if (s1 != NULL)
		free(s1);
	if (s2 != NULL)
		free(s2);
	return (final_str);
}

char	*expand_str(char *str, t_env *env_lst, int arg_nbr)
{
	char	*exp;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] != 0 && str[i] != '$')
		i++;
	j = i + 1;
	i++;
	while ((ft_isalnum(str[i]) == 1 || str[i] == '_') && str[i] != 0)
		i++;
	exp = expand_substr(ft_substr(str, j, i - j), env_lst, arg_nbr);
	if (!exp)
		return (NULL);
	temp = ft_strjoin_free (ft_substr(str, 0, j - 1), exp);
	if (!temp)
		return (NULL);
	temp = ft_strjoin_free(temp, ft_strdup(str + i));
	return (temp);
}

char	*exp_str_file(char *str, t_env *env_lst, char quote, int put_d)
{
	char	*exp;
	char	*temp;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (str[++i] != 0)
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) == 1 || str[i + 1] == '_'))
			break ;
	}
	j = ++i;
	while ((ft_isalnum(str[i]) == 1 || str[i] == '_') && str[i] != 0)
		i++;
	if (quote == '"' || str[i] != 0 || j > 1)
		put_d = 0;
	exp = expand_substr_file(ft_substr(str, j, i - j), env_lst, put_d);
	if (!exp)
		return (NULL);
	temp = ft_strjoin_free(ft_substr(str, 0, j - 1), exp);
	if (!temp)
		return (NULL);
	temp = ft_strjoin_free(temp, ft_strdup(str + i));
	return (temp);
}
