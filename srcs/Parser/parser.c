/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/28 03:19:46 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

extern int g_sigint;

void	set_original_root(t_ast *original_root, t_ast *root)
{
	if (!root)
		return ;
	set_original_root(original_root, root->left);
	set_original_root(original_root, root->subshell_ast);
	set_original_root(original_root, root->right);
	root->original_root = original_root;
}

int	parser(t_minishell *msh, t_token_list *token_list)
{
	t_ast			*root;
	t_command_table	*command_table;

	(void)msh;
	root = add_ast_node(&token_list, 0);
	if (!root)
		return (-1);
	set_original_root(root, root);
	command_table = NULL;
	create_command_table(msh, root, &command_table);
	if (!command_table)
		return (free_ast(root), -1);
	if (g_sigint)
	{
		g_sigint = !g_sigint;
		return (free_ast(root), free_command_table(command_table, 1), 130);
	}
	free_ast(root);
	return (expander(msh, command_table));
}
