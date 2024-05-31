/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:22:38 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/31 21:32:00 by jcameira         ###   ########.fr       */
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
	if (redirs->type == HERE_DOC)
	{
		if ((*root)->right->type == REDIRECTION)
		{
			redirs->here_doc_limiter = ft_strdup((*root)->right->left->content);
			(*root)->right->left->visited = 1;
		}
		else
		{
			redirs->here_doc_limiter = ft_strdup((*root)->right->content);
			(*root)->right->visited = 1;
		}
		if (!redirs->here_doc_limiter)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		redirs->file = NULL;
	}
	else
	{
		if ((*root)->right->type == REDIRECTION)
		{
			redirs->file = ft_strdup((*root)->right->left->content);
			(*root)->right->left->visited = 1;
		}
		else
		{
			redirs->file = ft_strdup((*root)->right->content);
			(*root)->right->visited = 1;
		}
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

void	add_simple_command_argument(t_ast *root, t_command_table **node)
{
	char	**new_array;
	int		size;

	size = array_size((*node)->simplecmd->arg_arr);
	new_array = malloc(sizeof(char *) * (size + 2));
	if (!new_array)
		return (ft_putstr_fd(NO_SPACE, 2));
	if ((*node)->simplecmd->arg_arr)
	{
		new_array = arrcpy(new_array, (*node)->simplecmd->arg_arr);
		if (!new_array)
			return ;
	}
	new_array[size] = ft_strdup(root->content);
	if (!new_array[size])
		return (ft_putstr_fd(NO_SPACE, 2));
	new_array[size + 1] = NULL;
	(*node)->simplecmd->arg_nbr++;
	(*node)->simplecmd->arg_arr = new_array;
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

void	add_more_content_to_table_node(t_ast **root, t_command_table **command_table)
{
	t_command_table	*last_node;
	t_redir_list	*last_redir;

	last_node = last_table_node(*command_table);
	last_redir = last_redir_node((*command_table)->redirs);
	if ((*root)->type == REDIRECTION)
		last_redir->next = new_command_table_redir(root);
	else if ((*root)->type == SIMPLE_COMMAND)
		add_simple_command_argument(*root, &last_node);
}

void	create_command_table(t_ast *root, t_command_table **command_table)
{
	t_command_table	*new_table_node;
	t_command_table	*last_node;

	if (!root)
		return ;
	create_command_table(root->left, command_table);
	if (root->subshell_ast)
		create_command_table(root->subshell_ast, command_table);
	else if (!root->visited)
	{
		last_node = last_table_node(*command_table);
		if ((root->type == REDIRECTION || root->type == SIMPLE_COMMAND) && *command_table && last_node->type == TABLE_NO_TYPE)
			add_more_content_to_table_node(&root, &last_node);
		else
		{
			new_table_node = add_command_table_node(root);
			if (!new_table_node)
				return ;
			add_new_table_node(command_table, new_table_node);
		}
		root->visited = 1;
	}
	if (!root)
		return ;
	create_command_table(root->right, command_table);
	return ;
}
