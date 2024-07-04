/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:36:42 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 18:40:12 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

int	isenvchar(int c)
{
	return (ft_isalnum(c) || c == 95);
}

char	*get_env_value(t_minishell *msh, char *env_name)
{
	char	**tmp_envp;
	char	*env_value;
	int		env_name_len;
	int		i;

	tmp_envp = msh->envp;
	env_name_len = ft_strlen(env_name);
	i = -1;
	while (tmp_envp[++i])
		if (!ft_strncmp(tmp_envp[i], env_name, env_name_len)
			&& tmp_envp[i][env_name_len] == '=')
			break ;
	if (!tmp_envp[i])
	{
		env_value = malloc(sizeof(char));
		if (!env_value)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		env_value[0] = '\0';
		return (env_value);
	}
	env_value = ft_substr(tmp_envp[i], env_name_len + 1,
			ft_strlen(&tmp_envp[i][env_name_len]));
	if (!env_value)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (env_value);
}

char	*get_env_name(char *content, int *i)
{
	int		env_name_start;
	char	*env_name;

	env_name_start = ++(*i);
	while (content[*i] && isenvchar(content[*i]))
		(*i)++;
	env_name = ft_substr(content, env_name_start, (*i) - env_name_start);
	if (!env_name)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	(*i)--;
	return (env_name);
}

int	get_env_variable_len(t_minishell *msh, char *content, int *i)
{
	int		env_len;
	char	*env_name;
	char	*env_value;

	env_name = get_env_name(content, i);
	if (!env_name)
		return (-1);
	env_value = get_env_value(msh, env_name);
	free(env_name);
	if (!env_value)
		return (-1);
	env_len = ft_strlen(env_value);
	free(env_value);
	return (env_len);
}
