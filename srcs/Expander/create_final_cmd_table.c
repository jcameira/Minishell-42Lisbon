/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_final_cmd_table.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 18:57:32 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/22 01:29:19 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

t_final_cmd_table	*set_redir_info(t_final_cmd_table *new_table_node,
	t_redir_list *redirs)
{
	if (redirs->type == HERE_DOC || redirs->type == INFILE)
	{
		new_table_node = set_redir_info_infile(new_table_node, redirs);
		if (!new_table_node)
			return (NULL);
	}
	else if (redirs->type == OUTFILE || redirs->type == APPEND)
	{
		new_table_node->out_type = redirs->type;
		if (new_table_node->outfile)
			free(new_table_node->outfile);
		new_table_node->outfile = ft_strdup(redirs->file);
		if (!new_table_node->outfile)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	return (new_table_node);
}

t_final_cmd_table	*new_f_cmd_table_node(t_command_table *command_table,
	t_symbol previous_symbol)
{
	t_final_cmd_table	*new_node;

	new_node = malloc(sizeof(t_final_cmd_table));
	if (!new_node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_node->subshell_level = command_table->subshell_level;
	new_node->simplecmd = simplecmdcpy(command_table->simplecmd);
	if (!new_node->simplecmd)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_node->builtin = builtin_arr(new_node->simplecmd->arg_arr[0]);
	new_node->ambiguous_redirect = 0;
	new_node = set_final_redirs(new_node, command_table->redirs);
	if (!new_node)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free_redir_list(command_table->redirs, 0);
	new_node->previous_symbol = previous_symbol;
	new_node->next_symbol = check_next_symbol(command_table->next);
	new_node->next = NULL;
	return (new_node);
}

t_final_cmd_table	*last_f_table_node(t_final_cmd_table *final_table)
{
	t_final_cmd_table	*tmp;

	if (!final_table)
		return (NULL);
	tmp = final_table;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_f_cmd_table_node(t_final_cmd_table *new_node,
	t_final_cmd_table **final_cmd_table)
{
	t_final_cmd_table	*last_node;

	if (!*final_cmd_table)
	{
		*final_cmd_table = new_node;
		return ;
	}
	last_node = last_f_table_node(*final_cmd_table);
	last_node->next = new_node;
}

t_final_cmd_table	*create_final_cmd_table(t_command_table *command_table)
{
	t_final_cmd_table		*final_cmd_table;
	t_final_cmd_table		*new_table_node;
	t_symbol				previous_symbol;
	t_command_table			*tmp;

	final_cmd_table = NULL;
	previous_symbol = NO_SYMBOL;
	while (command_table)
	{
		new_table_node = new_f_cmd_table_node(command_table, previous_symbol);
		if (!new_table_node)
		{
			free_f_command_table(final_cmd_table);
			return (free_command_table(command_table, 1), NULL);
		}
		previous_symbol = new_table_node->next_symbol;
		tmp = command_table->next;
		free(command_table);
		command_table = tmp;
		if (new_table_node->next_symbol != NO_SYMBOL)
			free_symbol_node(&command_table);
		add_f_cmd_table_node(new_table_node, &final_cmd_table);
	}
	return (final_cmd_table);
}
