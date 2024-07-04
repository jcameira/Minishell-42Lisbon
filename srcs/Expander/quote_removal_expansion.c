/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:24:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 18:58:03 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	quote_removal_str_len_aux(char *content, int *quotes, int *i,
	int *real_len)
{
	if (content[*i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, *i, '\'', QUOTES))
			*real_len += len_inside_quotes(content, i, '\'');
	}
	else if (content[*i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, *i, '"', QUOTES))
			*real_len += len_inside_quotes(content, i, '"');
	}
}

int	quote_removal_str_len(char *content)
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
			quote_removal_str_len_aux(content, quotes, &i, &real_len);
		else
			real_len++;
	}
	return (real_len);
}

void	quote_removal_expansion_aux(char *content, char *new_content,
	int *indexes, int *quotes)
{
	if (content[indexes[0]] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, indexes[0], '\'',
				QUOTES))
			add_content_inside_quotes(content, new_content, &indexes[0],
				&indexes[1]);
	}
	else if (content[indexes[0]] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, indexes[0], '"',
				QUOTES))
			add_content_inside_quotes(content, new_content, &indexes[0],
				&indexes[1]);
	}
}

char	*remove_quotes_expansion(char *content, int len)
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
			quote_removal_expansion_aux(content, new_content, indexes, quotes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
}
