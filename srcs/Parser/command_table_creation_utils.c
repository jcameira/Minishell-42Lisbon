/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:24:54 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 01:51:50 by jcameira         ###   ########.fr       */
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

t_redir_type	set_redir_type(char *content)
{
	if (!ft_strncmp(content, ">>", 2))
		return (APPEND);
	else if (!ft_strncmp(content, "<<", 2))
		return (HERE_DOC);
	else if (!ft_strncmp(content, ">", 2))
		return (OUTFILE);
	else
		return (INFILE);
}

char	*set_redir_str(char *dest, t_ast **root)
{
	if ((*root)->right->type == REDIRECTION)
	{
		dest = ft_strdup((*root)->right->left->content);
		(*root)->right->left->visited = 1;
	}
	else
	{
		dest = ft_strdup((*root)->right->content);
		(*root)->right->visited = 1;
	}
	if (!dest)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (dest);
}

t_redir_list	*set_redir_values(t_minishell *msh, t_ast **root,
	t_redir_list *redirs)
{
	redirs->type = set_redir_type((*root)->content);
	if (redirs->type == HERE_DOC)
	{
		redirs->here_doc_limiter = set_redir_str(redirs->here_doc_limiter,
				root);
		set_here_doc_expansion(&redirs);
		redirs->here_doc_limiter = remove_quotes(redirs->here_doc_limiter);
		handle_here_doc(msh, &redirs);
	}
	else
		redirs->file = set_redir_str(redirs->file, root);
	if (!redirs->here_doc_limiter && !redirs->file)
		return (NULL);
	return (redirs);
}
