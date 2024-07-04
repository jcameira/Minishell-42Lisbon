/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expansions_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:39:04 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 18:41:19 by jcameira         ###   ########.fr       */
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

char	*add_expanded_parameter(t_minishell *msh, char *content,
	char *new_content, int *indexes)
{
	char	*env_name;
	char	*env_value;

	env_name = get_env_name(content, &indexes[0]);
	if (!env_name)
	{
		free(content);
		return (free(new_content), NULL);
	}
	env_value = get_env_value(msh, env_name);
	if (!env_value)
	{
		free(env_name);
		free(content);
		return (free(new_content), NULL);
	}
	new_content = add_expanded_var(env_value, new_content, &indexes[1]);
	free(env_name);
	free(env_value);
	return (new_content);
}
