/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:19:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/10 18:45:36 by jcameira         ###   ########.fr       */
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
	new->content = ft_strdup(data);
	free(data);
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

void	add_token_middle_list(t_token_list **token_list, t_token_list *new)
{
	t_token_list	*tmp;

	if (!token_list)
		return ;
	if (!*token_list)
		*token_list = new;
	tmp = (*token_list)->next;
	(*token_list)->next = new;
	last_token(new)->next = tmp;
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

t_token_list	*set_token_types(t_token_list *token_list)
{
	t_token_list	*tmp;

	if (!token_list)
		return (NULL);
	tmp = token_list;
	while (tmp)
	{
		tmp->token_type = get_token_type(tmp->content);
		tmp = tmp->next;
	}
	return (token_list);
}

int	get_token_type(char	*c)
{
	if (*c == '&')
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
	if (!data || !(*data))
		return (NULL);
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
		tmp = get_initial_token_data(line, &i);
		if (tmp)
		{
			new = new_token(NO_TYPE, tmp);
			if (!new)
			{
				free_token_list(token_list);
				return (NULL);
			}
			add_new_token(&token_list, new);
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
		split_operator_tokens(&new);
		add_new_token(&new_list, new);
		tmp = tmp->next;
	}
	free_token_list(initial_list);
	return (new_list);
}

int	is_operator_token(char	*c)
{
	return (!ft_strncmp(c, "&&", 2) || *c == '|' || *c == '<'
		|| *c == '>' || *c == '(' || *c == ')');
}

void	skip_operator_token(t_token_list *node, int *i)
{
	if (!ft_strncmp(&(node->content[*i]), "&&", 2)
		|| !ft_strncmp(&(node->content[*i]), "||", 2)
		|| !ft_strncmp(&(node->content[*i]), "<<", 2)
		|| !ft_strncmp(&(node->content[*i]), ">>", 2))
		*i += 2;
	else if (node->content[*i] == '|' || node->content[*i] == '<'
		|| node->content[*i] == '>' || node->content[*i] == '('
		|| node->content[*i] == ')')
		(*i)++;
}
char	**split_by_position(char *line, int i)
{
	char	**arr;
	int		len;

	len = ft_strlen(line);
	arr = malloc(sizeof(char *) * 3);
	arr[0] = ft_substr(line, 0, i);
	arr[1] = ft_substr(line, i, len);
	arr[2] = NULL;
	return (arr);
}

void	execute_split(t_token_list **node, int *i)
{
	t_token_list	*back_node;
	char			**split_arr;

	split_arr = split_by_position((*node)->content, *i);
	free((*node)->content);
	(*node)->content = split_arr[0];
	back_node = new_token(NO_TYPE, split_arr[1]);
	add_token_middle_list(node, back_node);
}

void	split_node(t_token_list **node, int *i)
{
	// t_token_list	*tmp;

	// tmp = *node;
	if (*i == 0)
		skip_operator_token(*node, i);
	if ((*node)->content[*i])
	{
		execute_split(node, i);
		*node = (*node)->next;
		*i = 0;
	}
	// *node = tmp;
}

void	split_operator_tokens(t_token_list **node)
{
	t_token_list	*tmp;
	int	i;

	i = 0;
	tmp = *node;
	while ((*node)->content[i])
	{
		if ((*node)->content[i] == '\'')
			skip_until_char((*node)->content, &i, '\'');
		else if ((*node)->content[i] == '"')
			skip_until_char((*node)->content, &i, '"');
		else if(is_operator_token(&((*node)->content[i])))
		{
			split_node(node, &i);
			continue ;
		}
		i++;
	}
	*node = tmp;
}

void	free_token_list(t_token_list *list)
{
	t_token_list	*tmp;

	while(list)
	{
		tmp = list->next;
		free(list->content);
		free(list);
		list = tmp;
	}
}
