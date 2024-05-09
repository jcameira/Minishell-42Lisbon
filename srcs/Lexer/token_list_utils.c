/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:19:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/08 21:52:18 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_token_list	*new_token(int	type, char *data)
{
	t_token_list	*new;

	new = malloc(sizeof(t_token_list));
	if (!new)
		return (NULL);
	new->token_type = type;
	new->content = data;
	new->next = NULL;
	return (new);
}

void	add_new_token(t_token_list **token_list, t_token_list *new)
{
	t_token_list	*last;

	if (!token_list)
		return ;
	if (!*token_list)
	{
		*token_list = new;
		return ;
	}
	last = last_token(*token_list);
	last->next = new;
}

t_token_list	*last_token(t_token_list *token_list)
{
	t_token_list	*tmp;

	if (!token_list)
		return (NULL);
	tmp = token_list;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

// t_token_list	*set_token_type(t_token_list *token, int type)
// {
// 	if (!token)
// 		return (NULL);
// 	token->token_type = type;
// 	return (token);
// }

// int	get_token_type(char	*c)
// {
// 	if (*c == '&')
// 		return (AND);
// 	else if (*c == '|' && *(c + 1) == '|')
// 		return (OR);
// 	else if (*c == '|')
// 		return (PIPE);
// 	else if (*c == '<' && *(c + 1) == '<')
// 		return (D_LESSER);
// 	else if (*c == '<')
// 		return (LESSER);
// 	else if (*c == '>' && *(c + 1) == '>')
// 		return (D_GREATER);
// 	else if (*c == '>')
// 		return (GREATER);
// 	else if (*c == '(')
// 		return (L_PARENTESIS);
// 	else if (*c == ')')
// 		return (R_PARENTESIS);
// 	else if (ft_isprint(*c))
// 		return (WORD);
// 	else
// 		return (BAD_TOKEN);
// }

// int	iswhitespace(int c)
// {
// 	return ((c >= 9 && c <= 13) || c == 32);
// }

// char	*get_next_token(char *cmd_line)
// {
// 	int		i;
// 	bool	in_quotes;

// 	if (cmd_line[0] == '"')
// 	{
// 		i = 0;
// 		in_quotes = true;
// 	}
// 	else
// 	{
// 		i = -1;
// 		in_quotes = false;
// 	}
// 	while (cmd_line[++i])
// 	{
// 		if (iswhitespace(cmd_line[i]))
// 			continue ;
// 		else if (cmd_line[i] == '"' && in_quotes)
// 			in_quotes = false;
// 		else if (cmd_line[i] == '"' && !in_quotes)
// 			return (cmd_line + i);
// 		else if ((cmd_line[i] == '<' || cmd_line[i] == '>') && !in_quotes)
// 			return (cmd_line + i);
// 		else if ((cmd_line[i] == '|' || cmd_line[i] == '&') && !in_quotes)
// 			return (cmd_line + i);
// 		else if ((cmd_line[i] == '(' || cmd_line[i] == ')') && !in_quotes)
// 			return (cmd_line + i);
// 		else if (ft_isalnum(cmd_line[i]) && !in_quotes)
// 			return (cmd_line + i);
// 	}
// 	return (NULL);
// }

void	skip_until_char(char *line, int *i, char c)
{
	while(line[++(*i)] && line[*i] != c)
		continue ;
}

char	*get_initial_token_data(char *line, int *i)
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
	if (!(*data))
		return (NULL);
	return (data);
}

t_token_list	*get_initial_list(char *line)
{
	t_token_list	*token_list;
	char			*tmp;
	int				i;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && ft_strchr(WHITESPACE, line[i]))
			i++;
		tmp = get_initial_token_data(line, &i);
		if (tmp)
			add_new_token(&token_list, new_token(NO_TYPE, tmp));
		i++;
	}
	return (token_list);
}