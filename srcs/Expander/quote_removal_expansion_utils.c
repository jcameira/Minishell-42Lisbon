/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_expansion_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:38:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 18:40:16 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

int	expansion_inside_quotes(char *content, int i, char c, t_quote_flag flag)
{
	if (flag == QUOTES)
	{
		while (content[++i])
		{
			if (content[i] == c)
				break ;
			if (c == '\'' && (content[i] == '$' || content[i] == '*'))
				return (1);
			else if (c == '"' && content[i] == '*')
				return (1);
		}
	}
	else if (flag == PARAMETERS)
	{
		while (content[++i])
			if ((c == '"' || c == '\'') && content[i] == '*')
				return (1);
	}
	return (0);
}

int	len_inside_quotes(char *content, int *i, char c)
{
	int	len;

	len = 2;
	while (content[++(*i)] != c)
		len++;
	return (len);
}

void	add_content_inside_quotes(char *content, char *new_content, int *i,
	int *j)
{
	char	quote_type;

	quote_type = content[*i];
	new_content[++(*j)] = content[*i];
	while (content[++(*i)] != quote_type)
		new_content[++(*j)] = content[*i];
	new_content[++(*j)] = content[*i];
}
