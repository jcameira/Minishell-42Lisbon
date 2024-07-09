/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_final_command_table.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 18:57:32 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/10 00:38:29 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

t_final_command_table	*new_f_cmd_table_node(t_command_table *command_table)
{
	t_final_command_table	*new_table_node;

	new_table_node = malloc(sizeof(t_final_command_table));
	if (!new_table_node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_table_node->subshell_level = command_table->subshell_level;
	new_table_node->simplecmd = simplecmdcpy(command_table->simplecmd);
	if (!new_table_node->simplecmd)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_table_node = set_final_redirs(new_table_node, command_table->redirs);
	if (!new_table_node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free_redir_list(command_table->redirs);
	new_table_node->next_symbol = check_next_symbol(command_table->next);
	new_table_node->next = NULL;
	return (new_table_node);
}

t_final_command_table	*last_f_table_node(t_final_command_table *final_table)
{
	t_final_command_table	*tmp;

	if (!final_table)
		return (NULL);
	tmp = final_table;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_f_cmd_table_node(t_final_command_table *new_node,
	t_final_command_table **final_command_table)
{
	t_final_command_table	*last_node;

	if (!*final_command_table)
	{
		*final_command_table = new_node;
		return ;
	}
	last_node = last_f_table_node(*final_command_table);
	last_node->next = new_node;
}

t_final_command_table	*create_final_cmd_table(t_command_table *command_table)
{
	t_final_command_table	*final_command_table;
	t_final_command_table	*new_table_node;
	t_command_table			*tmp;

	final_command_table = NULL;
	while (command_table)
	{
		new_table_node = new_f_cmd_table_node(command_table);
		if (!new_table_node)
		{
			free_f_command_table(final_command_table);
			return (free_command_table(command_table), NULL);
		}
		tmp = command_table->next;
		free(command_table);
		command_table = tmp;
		add_f_cmd_table_node(new_table_node,
			&final_command_table);
	}
	return (final_command_table);
}
