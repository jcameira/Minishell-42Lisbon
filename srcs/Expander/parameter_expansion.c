/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:26:59 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 19:01:56 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	parameter_expansion_str_len_aux(char *content, int *quotes, int *i,
	int *real_len)
{
	static int	had_expansion_in_quotes;

	if (content[*i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, *i, '\'',
				PARAMETERS))
			*real_len += len_inside_quotes(content, i, '\'');
	}
	else if (content[*i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, *i, '"',
				PARAMETERS))
		{
			had_expansion_in_quotes = !had_expansion_in_quotes;
			(*real_len)++;
		}
		if (!quotes[D] && had_expansion_in_quotes)
		{
			had_expansion_in_quotes = !had_expansion_in_quotes;
			(*real_len)++;
		}
	}
}

int	parameter_expansion_str_len(t_minishell *msh, char *content)
{
	int	real_len;
	int	quotes[2];
	int	i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' || content[i] == '"')
			parameter_expansion_str_len_aux(content, quotes, &i, &real_len);
		else if (content[i] == '$' && content[i + 1]
			&& isenvchar(content[i + 1]) && !quotes[S])
			real_len += get_env_variable_len(msh, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

void	update_new_content(char *content, char **new_content, int *indexes,
	int *had_expansion_in_quotes)
{
	*had_expansion_in_quotes = !(*had_expansion_in_quotes);
	(*new_content)[++indexes[1]] = content[indexes[0]];
}

void	check_parameter_quotes(char *content, char **new_content, int *indexes,
	int *quotes)
{
	static int	had_expansion_in_quotes;

	if (content[indexes[0]] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, indexes[0], '\'',
				PARAMETERS))
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
		if (!quotes[S] && had_expansion_in_quotes)
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
	}
	else if (content[indexes[0]] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, indexes[0], '"',
				PARAMETERS))
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
		if (!quotes[D] && had_expansion_in_quotes)
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
	}
}

char	*expand_parameter(t_minishell *msh, char *content, int len)
{
	char	*new_content;
	int		quotes[2];
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	quotes[S] = 0;
	quotes[D] = 0;
	indexes[0] = -1;
	indexes[1] = -1;
	while (content[++indexes[0]])
	{
		if (content[indexes[0]] == '\'' || content[indexes[0]] == '"')
			check_parameter_quotes(content, &new_content, indexes, quotes);
		else if (content[indexes[0]] == '$' && content[indexes[0] + 1]
			&& content[indexes[0] + 1] != ' ' && !quotes[S])
			new_content = add_expanded_parameter(msh, content, new_content,
					indexes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
}
