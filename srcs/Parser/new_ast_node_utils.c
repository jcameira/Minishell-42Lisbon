/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast_node_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:41:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/17 12:43:12 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

int	check_for_node(t_token_list *token_list, t_ast_token_type type)
{
	t_token_list	*tmp;

	tmp = token_list;
	while (tmp)
	{
		if (tmp->token_type == L_PARENTESIS)
			skip_subshell(&tmp);
		else if (type == AND_OR_SEQUENCE && (tmp->token_type == AND
				|| tmp->token_type == OR))
			return (1);
		else if (type == PIPE_SEQUENCE && tmp->token_type == PIPE)
			return (1);
		else if (type == REDIRECTION && (tmp->token_type == LESSER
				|| tmp->token_type == D_LESSER || tmp->token_type == GREATER
				|| tmp->token_type == D_GREATER))
			return (1);
		else if (type == SIMPLE_COMMAND && tmp->token_type == WORD)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	check_for_subshell(t_token_list *token_list)
{
	t_token_list	*tmp;
	int				parentesis;

	if (token_list->token_type != L_PARENTESIS)
		return (0);
	tmp = token_list->next;
	parentesis = 1;
	while (tmp)
	{
		if (tmp->token_type == L_PARENTESIS)
			parentesis++;
		if (tmp->token_type == R_PARENTESIS)
			parentesis--;
		tmp = tmp->next;
	}
	if (parentesis)
		return (0);
	return (1);
}

void	trim_parentesis_nodes(t_token_list **token_list)
{
	t_token_list	*tmp;

	free_token_list_node(token_list);
	tmp = *token_list;
	while (tmp->next->next)
		tmp = tmp->next;
	free_token_list_node(&tmp->next);
	tmp->next = NULL;
}
