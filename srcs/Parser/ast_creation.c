/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 22:40:43 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/31 21:08:11 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_ast	*add_ast_node(t_token_list **token_list, int subshell_lvl)
{
	t_ast		*node;

	if (!token_list || !*token_list)
		return (NULL);
	node = NULL;
	if (check_for_node(*token_list, AND_OR_SEQUENCE))
		node = new_regular_node(token_list, AND_OR_SEQUENCE, subshell_lvl);
	else if (check_for_node(*token_list, PIPE_SEQUENCE))
		node = new_regular_node(token_list, PIPE_SEQUENCE, subshell_lvl);
	else if (check_for_subshell(*token_list))
		node = new_subshell_node(token_list, ++subshell_lvl);
	else if (check_for_node(*token_list, REDIRECTION))
		node = new_regular_node(token_list, REDIRECTION, subshell_lvl);
	else if (check_for_node(*token_list, SIMPLE_COMMAND))
		node = new_regular_node(token_list, SIMPLE_COMMAND, subshell_lvl);
	return (node);
}

t_ast	*new_regular_node(t_token_list **token_list, t_ast_token_type type, int subshell_lvl)
{
	t_ast			*node;
	t_token_list	*target_list_node;
	t_token_list	*left_list;
	t_token_list	*right_list;

	target_list_node = search_list_for_token(*token_list, type);
	node = new_ast_node(target_list_node, subshell_lvl);
	if (!node)
		return (NULL);
	separate_list(token_list, &left_list, &right_list,
		target_list_node->token_type);
	node->left = add_ast_node(&left_list, subshell_lvl);
	node->right = add_ast_node(&right_list, subshell_lvl);
	return (node);
}

t_ast	*new_subshell_node(t_token_list **token_list, int subshell_lvl)
{
	t_token_list	*tmp;
	t_token_list	*tmp2;
	t_ast			*node;

	tmp = *token_list;
	skip_subshell(&tmp);
	tmp2 = tmp;
	tmp = tmp->next;
	tmp2->next = NULL;
	node = new_ast_node(*token_list, subshell_lvl);
	if (!node)
		return (NULL);
	trim_parentesis_nodes(token_list);
	node->subshell_ast = add_ast_node(token_list, subshell_lvl);
	node->right = add_ast_node(&tmp, subshell_lvl);
	return (node);
}
