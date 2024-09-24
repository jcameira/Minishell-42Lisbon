/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expansions_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:39:04 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 20:02:49 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

int	needs_quote_removal(char *content)
{
	int	i;

	i = -1;
	while (content[++i])
		if (content[i] == '\'' || content[i] == '"')
			return (1);
	return (0);
}

void	set_here_doc_expansion(t_redir_list **redir)
{
	int	i;

	i = -1;
	while ((*redir)->here_doc_limiter[++i])
	{
		if ((*redir)->here_doc_limiter[i] == '\''
			|| (*redir)->here_doc_limiter[i] == '"')
		{
			(*redir)->expand_here_doc = 1;
			break ;
		}
	}
}

char	*remove_quotes(char *content)
{
	char	*quotes_removed;
	int		real_len;

	if (!needs_quote_removal(content))
		return (content);
	real_len = str_len_no_quotes(content);
	quotes_removed = set_no_quotes_content(content, real_len);
	if (!quotes_removed)
		return (NULL);
	free(content);
	return (quotes_removed);
}

char	*add_expanded_var(char *env_value, char *expanded_content, int *j)
{
	int		i;

	i = -1;
	while (env_value[++i])
		expanded_content[++(*j)] = env_value[i];
	return (expanded_content);
}

char	*add_expanded_parameter(t_minishell *msh, t_command_table *table,
	char **contents, int (*indexes)[2])
{
	int		tmp_i;
	char	*env_name;
	char	*env_value;

	tmp_i = (*indexes)[0];
	env_name = get_env_name(contents[0], indexes[0]);
	if (!env_name)
		return (free(contents[0]), free(contents[1]), NULL);
	if (!ft_strcmp(env_name, "PATH") && msh->private_path)
		env_value = ft_strdup(msh->private_path);
	else if (contents[0][tmp_i + 1] == '?')
		env_value = ft_itoa(msh->exit_code);
	else if (!ft_strcmp(env_name, EXPAND_SUBSHELL))
		env_value = ft_itoa(table->subshell_level);
	else
		env_value = get_env_value(msh, env_name);
	if (!env_value)
		return (free(env_name), free(contents[0]), free(contents[1]), NULL);
	contents[1] = add_expanded_var(env_value, contents[1], &(*indexes)[1]);
	free(env_name);
	free(env_value);
	if (contents[0][tmp_i + 1] == '?')
		(*indexes)[0] = tmp_i + 1;
	return (contents[1]);
}
