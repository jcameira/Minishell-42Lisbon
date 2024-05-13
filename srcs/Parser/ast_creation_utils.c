/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:33:02 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 16:48:01 by jcameira         ###   ########.fr       */
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
	if (!strncmp(new->type, PRINT_SUBSHELL, ft_strlen(PRINT_SUBSHELL)))
		new->content = get_subshell_content(token_node);
	else
		new->content = get_node_content(token_node);
	new->subshell_ast = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}
