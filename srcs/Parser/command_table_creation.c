/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:22:38 by jcameira          #+#    #+#             */
/*   Updated: 2024/06/01 14:36:28 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_redir_list	*new_command_table_redir(t_ast **root)
{
	t_redir_list	*redirs;

	redirs = malloc(sizeof(t_redir_list));
	if (!redirs)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	redirs->next = NULL;
	redirs->file = NULL;
	redirs->here_doc_limiter = NULL;
	if ((*root)->type == REDIRECTION)
	{
		redirs->type = set_redir_type((*root)->content);
		if (redirs->type == HERE_DOC)
			redirs->here_doc_limiter = set_redir_str(redirs->here_doc_limiter,
					root);
		else
			redirs->file = set_redir_str(redirs->file, root);
		if (!redirs->here_doc_limiter && !redirs->file)
			return (NULL);
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

void	add_more_content_to_table_node(t_ast **root,
		t_command_table **command_table)
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
		if ((root->type == REDIRECTION || root->type == SIMPLE_COMMAND)
			&& *command_table && last_node->type == TABLE_NO_TYPE)
			add_more_content_to_table_node(&root, &last_node);
		else
		{
			new_table_node = new_command_table_node(root);
			if (!new_table_node)
				return ;
			add_new_table_node(command_table, new_table_node);
		}
		root->visited = 1;
	}
	if (root)
		create_command_table(root->right, command_table);
}
