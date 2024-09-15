/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_command_table_node_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:29:52 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/14 15:45:20 by jcameira         ###   ########.fr       */
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

t_command_table	*new_command_table_node(t_minishell *msh, t_ast *root, t_command_table *command_table)
{
	t_command_table	*node;

	(void)msh;
	(void)command_table;
	node = malloc(sizeof(t_command_table));
	if (!node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	node->type = set_table_node_type(root);
	node->subshell_level = root->subshell_level;
	// node->simplecmd = new_command_table_simple_command(root);
	node->simplecmd = NULL;
	// if (!node->simplecmd)
	// 	return (NULL);
	//node->redirs = new_command_table_redir(msh, &root, command_table);
	node->redirs = NULL;
	// if (!node->redirs)
	// 	return (NULL);
	node->next = NULL;
	return (node);
}
