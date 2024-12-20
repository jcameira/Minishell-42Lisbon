/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_command_table_node_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:29:52 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 15:52:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_command_table	*last_table_node(t_command_table *command_table)
{
	t_command_table	*tmp;

	if (!command_table)
		return (NULL);
	tmp = command_table;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_redir_list	*last_redir_node(t_redir_list *redir_list)
{
	t_redir_list	*tmp;

	if (!redir_list)
		return (NULL);
	tmp = redir_list;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_new_table_node(t_command_table **command_table,
		t_command_table *new)
{
	t_command_table	*last;

	if (!command_table)
		return ;
	if (!*command_table)
	{
		*command_table = new;
		return ;
	}
	last = last_table_node(*command_table);
	last->next = new;
}

t_command_table	*new_command_table_node(t_ast *root)
{
	t_command_table	*node;

	node = malloc(sizeof(t_command_table));
	if (!node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	node->type = set_table_node_type(root);
	node->subshell_level = root->subshell_level;
	node->simplecmd = NULL;
	node->redirs = NULL;
	node->next = NULL;
	return (node);
}
