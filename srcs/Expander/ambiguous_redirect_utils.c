/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous_redirect_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:46:26 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 23:16:09 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

int	has_space(char *content)
{
	int		i;

	i = -1;
	while (content[++i])
		if (content[i] == ' ')
			return (1);
	return (0);
}

int	space_not_in_quotes(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '"')
		{
			while (str[++i] != '"')
				continue ;
		}
		else if (str[i] == '\'')
		{
			while (str[++i] != '\'')
				continue ;
		}
		else if (str[i] == ' ')
			return (1);
	}
	return (0);
}

int	space_in_quotes(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != '"')
			continue ;
		else
		{
			while (str[++i] != '"')
				if (str[i] == ' ')
					return (1);
		}
	}
	return (0);
}

int	set_ambiguous_redirect(t_minishell *msh, t_final_cmd_table *table,
	char *content)
{
	char	*expanded_content;
	int		expanded_len;

	expanded_content = ft_strdup(content);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), -1);
	expanded_len = parameter_expansion_str_len(msh, table, expanded_content);
	expanded_content = expand_parameter(msh, table, expanded_content,
			expanded_len);
	if (!expanded_content)
		return (free(expanded_content), -1);
	if (space_in_quotes(expanded_content))
		return (free(expanded_content), 0);
	expanded_len = wildcards_str_len(expanded_content);
	expanded_content = expand_wildcards(expanded_content, expanded_len,
			needs_wildcard_expansion(expanded_content));
	if (!expanded_content)
		return (free(expanded_content), -1);
	if (space_in_quotes(expanded_content))
		return (free(expanded_content), 0);
	if (!has_space(expanded_content))
		return (free(expanded_content), 0);
	return (free(expanded_content), 1);
}
