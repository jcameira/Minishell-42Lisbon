/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_command_table_node_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:29:52 by jcameira          #+#    #+#             */
/*   Updated: 2024/06/21 17:14:55 by jcameira         ###   ########.fr       */
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
	t_command_table	*command_table;

	command_table = malloc(sizeof(t_command_table));
	if (!command_table)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	command_table->type = set_table_node_type(root);
	command_table->subshell_level = root->subshell_level;
	command_table->simplecmd = new_command_table_simple_command(root);
	if (!command_table->simplecmd)
		return (NULL);
	command_table->redirs = new_command_table_redir(&root);
	if (!command_table->redirs)
		return (NULL);
	command_table->next = NULL;
	return (command_table);
}
