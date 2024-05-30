/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:22:38 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/30 02:21:15 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_table_node_type	set_table_node_type(t_ast *root)
{
	if (!ft_strncmp(root->content, "&&", 2))
		return (TABLE_AND);
	else if (!ft_strncmp(root->content, "||", 2))
		return (TABLE_OR);
	else if (root->type == PIPE_SEQUENCE)
		return (TABLE_PIPE);
	else if (root->type == SUBSHELL)
		return (TABLE_SUBSHELL);
	else
		return (TABLE_NO_TYPE);
}

t_command_table	*add_command_table_node(t_ast *root)
{
	t_command_table	*command_table;
	static int		subshell_lvl;

	command_table = malloc(sizeof(t_command_table));
	if (!command_table)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	command_table->type = set_table_node_type(root);
	if (command_table->type == SUBSHELL)
		subshell_lvl++;
	return (command_table);
}

t_command_table	*create_command_table(t_ast *root)
{
	t_command_table	*command_table;

	if (!root)
		return (NULL);
	command_table = add_command_table_node(root);
	if (!command_table)
		return (NULL);
	return (command_table);
}
