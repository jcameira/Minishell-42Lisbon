/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:24:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 01:32:44 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	quote_removal_expansion_aux(char *content, int *i, int *quotes)
{
	if (content[*i] == '\'' && !quotes[D])
		quotes[S] = !quotes[S];
	else if (content[*i] == '"' && !quotes[S])
		quotes[D] = !quotes[D];
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
			quote_removal_expansion_aux(content, &i, quotes);
		else
			real_len++;
	}
	return (real_len);
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
			quote_removal_expansion_aux(content, &indexes[0], quotes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
}
