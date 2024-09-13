/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory_handle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:12:52 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/13 20:46:26 by jcameira         ###   ########.fr       */
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

void	free_command_table(t_command_table *command_table)
{
	t_command_table	*tmp;
	t_redir_list	*tmp_redir;

	while (command_table)
	{
		tmp = command_table->next;
		free_arr(command_table->simplecmd->arg_arr);
		free(command_table->simplecmd);
		while (command_table->redirs)
		{
			printf("%p\n", command_table->redirs->next);
			tmp_redir = command_table->redirs->next;
			if (command_table->redirs->file)
			{
				printf("AAAAAAAAAAAAAAAAAAAAAAAAAA\n");
				free(command_table->redirs->file);
			}
			if (command_table->redirs->here_doc_limiter)
			{
				printf("BBBBBBBBBBBBBBBBBBBBBBBBBB\n");
				free(command_table->redirs->here_doc_limiter);
			}
			free(command_table->redirs);
			command_table->redirs = tmp_redir;
		}
		free(command_table);
		command_table = tmp;
	}
}
