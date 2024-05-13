/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 22:40:43 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 23:48:36 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_ast	*create_ast(t_token_list **token_list)
{
	t_ast	*node;

	if (!token_list)
		return (NULL);
	node = NULL;
	if (check_full_cmd_line_subshell(*token_list))
	{
		node = new_ast_node(*token_list);
		if (!node)
			return (NULL);
		trim_parentesis_nodes(token_list);
	}
	return (node);
}
