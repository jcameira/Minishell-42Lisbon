/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:33:02 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/22 13:34:33 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_ast	*new_ast_node(t_token_list *token_node)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new->type = set_ast_node_type(token_node);
	if (new->type == SUBSHELL)
		new->content = get_subshell_content(token_node);
	else
		new->content = get_node_content(token_node);
	if (!new->content)
		return (NULL);
	new->subshell_ast = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	skip_subshell(t_token_list **token_list)
{
	int	parentesis;

	parentesis = 1;
	while ((*token_list)->next && parentesis)
	{
		if ((*token_list)->next->token_type == L_PARENTESIS)
			parentesis++;
		if ((*token_list)->next->token_type == R_PARENTESIS)
			parentesis--;
		*token_list = (*token_list)->next;
	}
}

t_token_list	*search_list_for_token(t_token_list *token_list,
					t_ast_token_type type)
{
	t_token_list	*tmp;
	t_token_list	*target;

	tmp = token_list;
	target = NULL;
	while (tmp)
	{
		if (tmp->token_type == L_PARENTESIS)
			skip_subshell(&tmp);
		else if (type == AND_OR_SEQUENCE && (tmp->token_type == AND
				|| tmp->token_type == OR))
			target = tmp;
		else if (type == PIPE_SEQUENCE && tmp->token_type == PIPE)
			target = tmp;
		else if (type == REDIRECTION && (tmp->token_type == LESSER
				|| tmp->token_type == D_LESSER || tmp->token_type == GREATER
				|| tmp->token_type == D_GREATER))
			target = tmp;
		else if (type == SIMPLE_COMMAND && tmp->token_type == WORD)
			target = tmp;
		if (target)
			break ;
		tmp = tmp->next;
	}
	return (target);
}

void	separate_list(t_token_list **token_list, t_token_list **left_list,
			t_token_list **right_list, t_token_list_type type)
{
	t_token_list	*previous;

	*left_list = *token_list;
	previous = NULL;
	while ((*token_list))
	{
		if ((*token_list)->token_type == type)
		{
			if (previous)
				previous->next = NULL;
			else
				*left_list = NULL;
			*right_list = (*token_list)->next;
			free_token_list_node(token_list);
			return ;
		}
		previous = *token_list;
		*token_list = (*token_list)->next;
	}
}
