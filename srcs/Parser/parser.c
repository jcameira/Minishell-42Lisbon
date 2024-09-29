/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/29 01:57:08 by jcameira         ###   ########.fr       */
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

void	print_cmd_table(t_command_table *command_table)
{
	t_redir_list	*tmp_redir;
	int				i;

	while (command_table)
	{
		printf("\nTYPE:	%d	SUBSHELL_LVL:	%d\n", command_table->type,
			command_table->subshell_level);
		i = -1;
		printf("\nCommand\n");
		if (command_table->simplecmd)
		{
			while (command_table->simplecmd->arg_arr[++i])
				printf("%s->", command_table->simplecmd->arg_arr[i]);
			printf("\n");
		}
		printf("\nRedirections\n");
		tmp_redir = command_table->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == HERE_DOC)
				printf("'<<'\n%s\n", tmp_redir->here_doc_limiter);
			else if (tmp_redir->type == INFILE)
				printf("'<'\n%s\n", tmp_redir->file);
			else if (tmp_redir->type == APPEND)
				printf("'>>'\n%s\n", tmp_redir->file);
			else if (tmp_redir->type == OUTFILE)
				printf("'>'\n%s\n", tmp_redir->file);
			else
				printf("Empty redir node\n");
			tmp_redir = tmp_redir->next;
		}
		printf("\n");
		command_table = command_table->next;
	}
}

int	parser(t_minishell *msh, t_token_list *token_list)
{
	t_ast					*root;
	t_command_table			*command_table;
	t_final_cmd_table		*final_cmd_table;

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
	print_cmd_table(command_table);
	final_cmd_table = create_final_cmd_table(msh, command_table);
	// return (expander(msh, command_table));
	return (executor(msh, final_cmd_table));
}
