/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_creation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:19:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 15:12:56 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_token_list_type	set_token_type(char	*c)
{
	if (*c == '&' && *(c + 1) == '&')
		return (AND);
	else if (*c == '|' && *(c + 1) == '|')
		return (OR);
	else if (*c == '|')
		return (PIPE);
	else if (*c == '<' && *(c + 1) == '<')
		return (D_LESSER);
	else if (*c == '<')
		return (LESSER);
	else if (*c == '>' && *(c + 1) == '>')
		return (D_GREATER);
	else if (*c == '>')
		return (GREATER);
	else if (*c == '(')
		return (L_PARENTESIS);
	else if (*c == ')')
		return (R_PARENTESIS);
	else if (ft_isprint(*c))
		return (WORD);
	else
		return (BAD_TOKEN);
}

void	skip_until_char(char *line, int *i, char c)
{
	while (line[++(*i)] && line[*i] != c)
		continue ;
}

static char	*get_initial_token_data(char *line, int *i)
{
	char	*data;
	int		data_start;

	data_start = *i;
	while (line[*i] && !ft_strchr(WHITESPACE, line[(*i)]))
	{
		if (line[*i] == '\'')
			skip_until_char(line, i, '\'');
		if (line[*i] == '"')
			skip_until_char(line, i, '"');
		(*i)++;
	}
	data = ft_substr(line, data_start, (*i) - data_start);
	if (!data)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (data);
}

t_token_list	*get_initial_list(char *line)
{
	t_token_list	*token_list;
	t_token_list	*new;
	char			*tmp;
	int				i;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && ft_strchr(WHITESPACE, line[i]))
			i++;
		if (line[i])
		{
			tmp = get_initial_token_data(line, &i);
			if (!tmp)
				return (free_token_list(token_list), NULL);
			new = new_token(NO_TYPE, tmp);
			if (!new)
				return (free_token_list(token_list), NULL);
			add_new_token(&token_list, new);
			free(tmp);
		}
	}
	return (token_list);
}

t_token_list	*refine_list(t_token_list *initial_list)
{
	t_token_list	*new_list;
	t_token_list	*tmp;
	t_token_list	*new;

	new_list = NULL;
	tmp = initial_list;
	while (tmp)
	{
		new = new_token(NO_TYPE, tmp->content);
		if (!new)
		{
			free_token_list(initial_list);
			return (free_token_list(new_list), NULL);
		}
		new = split_operator_tokens(new);
		if (!new)
		{
			free_token_list(initial_list);
			return (free_token_list(new_list), NULL);
		}
		add_new_token(&new_list, new);
		tmp = tmp->next;
	}
	free_token_list(initial_list);
	return (new_list);
}
