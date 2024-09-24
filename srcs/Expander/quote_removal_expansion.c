/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:24:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 16:46:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	quote_removal_expansion_aux(char *content, int *i, int *quotes, int *real_len)
{
	if (content[*i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		return ;
	}
	else if (content[*i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		return ;
	}
	(*real_len)++;
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
			quote_removal_expansion_aux(content, &i, quotes, &real_len);
		else
			real_len++;
		printf("Cycle %d - S = %d, D = %d, Len %d\n", i + 1, quotes[S], quotes[D], real_len);
	}
	printf("%d\n", real_len);
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
			quote_removal_expansion_aux(content, &indexes[0], quotes, NULL);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
}
