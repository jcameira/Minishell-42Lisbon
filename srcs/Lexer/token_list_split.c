/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:18:41 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 22:00:08 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static void	skip_operator_token(t_token_list *node, int *i)
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

static char	**split_by_position(char *line, int i)
{
	char	**arr;
	int		len;

	len = ft_strlen(line);
	arr = malloc(sizeof(char *) * 3);
	if (!arr)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	arr[0] = ft_substr(line, 0, i);
	if (!arr[0])
	{
		free(arr);
		return (NULL);
	}
	arr[1] = ft_substr(line, i, len);
	if (!arr[1])
	{
		free(arr[0]);
		free(arr);
		return (NULL);
	}
	arr[2] = NULL;
	return (arr);
}

static t_token_list	*execute_split(t_token_list *node, int *i)
{
	t_token_list	*back_node;
	char			**split_arr;

	split_arr = split_by_position(node->content, *i);
	if (!split_arr)
		return (free(node->content), NULL);
	free(node->content);
	node->content = ft_strdup(split_arr[0]);
	back_node = new_token(NO_TYPE, split_arr[1]);
	if (!back_node)
		return (free_arr(split_arr), NULL);
	free_arr(split_arr);
	add_token_middle_list(&node, back_node);
	return (node);
}

static t_token_list	*split_node(t_token_list *node, int *i)
{
	if (*i == 0)
		skip_operator_token(node, i);
	if (node->content[*i])
	{
		node = execute_split(node, i);
		if (!node)
			return (NULL);
		node = node->next;
		*i = 0;
	}
	return (node);
}

t_token_list	*split_operator_tokens(t_token_list *node)
{
	t_token_list	*tmp;
	int				i;

	i = 0;
	tmp = node;
	while (node->content[i])
	{
		if (node->content[i] == '\'')
			skip_until_char(node->content, &i, '\'');
		else if (node->content[i] == '"')
			skip_until_char(node->content, &i, '"');
		else if (is_operator_token(&(node->content[i])))
		{
			node = split_node(node, &i);
			continue ;
		}
		i++;
	}
	return (tmp);
}
