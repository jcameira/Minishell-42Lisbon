/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:36:42 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 15:47:57 by marvin           ###   ########.fr       */
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

int	get_env_variable_len(t_minishell *msh, t_command_table *table,
	char *content, int *i)
{
	int		env_len;
	int		tmp_i;
	char	*env_name;
	char	*env_value;

	tmp_i = *i;
	env_name = get_env_name(content, i);
	if (!env_name && content[tmp_i + 1] != '?')
		return (-1);
	if (!ft_strcmp(env_name, "PATH") && msh->private_path)
		env_value = ft_strdup(msh->private_path);
	else if (content[tmp_i + 1] == '?')
		env_value = ft_itoa(msh->exit_code);
	else if (!ft_strcmp(env_name, EXPAND_SUBSHELL))
		env_value = ft_itoa(table->subshell_level);
	else
		env_value = get_env_value(msh, env_name);
	free(env_name);
	if (!env_value)
		return (-1);
	env_len = ft_strlen(env_value);
	free(env_value);
	if (content[tmp_i + 1] == '?')
		*i = tmp_i + 1;
	return (env_len);
}
