/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory_handle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:12:52 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/14 17:25:09 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	free_token_list_node(t_token_list **node)
{
	free((*node)->content);
	free(*node);
}

void	free_ast(t_ast *root)
{
	if (!root)
		return ;
	free_ast(root->left);
	free_ast(root->subshell_ast);
	free_ast(root->right);
	free(root->content);
	free(root);
}

void	free_command_table(t_command_table *command_table, int close_all_fds)
{
	t_command_table	*tmp;

	while (command_table)
	{
		tmp = command_table->next;
		free_arr(command_table->simplecmd->arg_arr);
		free(command_table->simplecmd);
		free_redir_list(command_table->redirs, close_all_fds);
		free(command_table);
		command_table = tmp;
	}
}
