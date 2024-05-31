/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:07:27 by tsaari            #+#    #+#             */
/*   Updated: 2024/05/31 15:42:09 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_substr(char *str, t_env_lst *env_lst)
{
	char	*temp;

	if (!str || !env_lst)
		return (NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
		temp = ft_strdup("");
	free(str);
	return (temp);
}

static char	*expand_substr_file(char *str, t_env_lst *env_lst)
{
	char	*temp;

	if (!str || !env_lst)
		return (NULL);
	if (expand_env_var(env_lst, str) != NULL)
		temp = ft_strdup(expand_env_var(env_lst, str));
	else
	{
		temp = ft_strjoin("$", str);
	}
	return (temp);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*final_str;

	if (s1 == 0 || s2 == 0)
		return (0);
	i = 0;
	j = 0;
	final_str = (char *) malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (final_str == 0)
		return (0);
	while (s1[i] != '\0')
	{
		final_str[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
		final_str[j++] = s2[i++];
	final_str[j] = '\0';
	free(s1);
	free(s2);
	return (final_str);
}

char	*expand_str(char *str, t_env_lst *env_lst)
{
	char	*new;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i] != 0 && str[i] != '$')
		i++;
	ft_strlcpy(new, str, i - j + 1);
	j = i + 1;
	i++;
	while ((ft_isalnum(str[i]) == 1 || str[i] == '_') && str[i] != 0)
		i++;
	temp = ft_strjoin_free (ft_strdup(new), \
	expand_substr(ft_substr(str, j, i - j), env_lst));
	free (new);
	if (!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin_free(temp, ft_substr(str + j, 0, i));
	return (temp);
}

char	*expand_str_file(char *str, t_env_lst *env_lst)
{
	char	*new;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i] != 0 && str[i] != '$')
		i++;
	ft_strlcpy(new, str, i - j + 1);
	j = i + 1;
	i++;
	while (ft_isalnum(str[i]) == 1 && str[i] != 0)
		i++;
	temp = ft_strjoin_free(ft_strdup(new), \
	expand_substr_file(ft_substr(str, j, i - j), env_lst));
	free (new);
	if (!temp)
		return (NULL);
	j = i;
	while (str[i] != 0)
		i++;
	temp = ft_strjoin(temp, ft_substr(str + j, 0, i));
	return (temp);
}
