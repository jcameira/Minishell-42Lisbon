/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:22:38 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/30 23:14:43 by jcameira         ###   ########.fr       */
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

t_redir_list	*new_command_table_redir(t_ast **root)
{
	t_redir_list	*redirs;

	redirs = malloc(sizeof(t_redir_list));
	if (!redirs)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	redirs->next = NULL;
	if ((*root)->type != REDIRECTION)
	{
		redirs->file = NULL;
		redirs->here_doc_limiter = NULL;
		return (redirs);
	}
	redirs->type = set_redir_type((*root)->content);
	*root = (*root)->right;
	if (redirs->type == HERE_DOC)
	{
		redirs->here_doc_limiter = ft_strdup((*root)->content);
		if (!redirs->here_doc_limiter)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		redirs->file = NULL;
	}
	else
	{
		redirs->file = ft_strdup((*root)->content);
		if (!redirs->file)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		redirs->here_doc_limiter = NULL;
	}
	return (redirs);
}

t_simplecmd	*new_command_table_simple_command(t_ast *root)
{
	t_simplecmd	*simplecmd;

	simplecmd = malloc(sizeof(t_redir_list));
	if (!simplecmd)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	if (root->type != SIMPLE_COMMAND)
	{
		simplecmd->arg_nbr = 0;
		simplecmd->arg_arr = NULL;
		return (simplecmd);
	}
	simplecmd->arg_nbr = 1;
	simplecmd->arg_arr = malloc(sizeof(char *) * 2);
	if (!simplecmd->arg_arr)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	simplecmd->arg_arr[simplecmd->arg_nbr - 1] = ft_strdup(root->content);
	if (!simplecmd->arg_arr[simplecmd->arg_nbr - 1])
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	simplecmd->arg_arr[simplecmd->arg_nbr] = NULL;
	return (simplecmd);
}

t_command_table	*add_command_table_node(t_ast *root)
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

void	add_new_table_node(t_command_table **command_table, t_command_table *new)
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

void	create_command_table(t_ast *root, t_command_table **command_table)
{
	t_command_table	*new_table_node;

	if (!root)
		return ;
	create_command_table(root->left, command_table);
	if (root->subshell_ast)
		create_command_table(root->subshell_ast, command_table);
	else
	{
		// if ((root->type == REDIRECTION || root->type == SIMPLE_COMMAND) && last_table_node(command_table)->type == TABLE_NO_TYPE)
		// 	add_more_content_to_table_node(root, command_table);
		// else
		// {
			new_table_node = add_command_table_node(root);
			if (!new_table_node)
				return ;
			add_new_table_node(command_table, new_table_node);
		// }
	}
	create_command_table(root->right, command_table);
	return ;
}
