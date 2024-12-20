/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_final_cmd_table_utils.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 00:37:59 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/22 01:29:14 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

t_final_cmd_table	*set_redir_info_infile(t_final_cmd_table *node,
	t_redir_list *redirs)
{
	node->in_type = redirs->type;
	if (node->infile)
	{
		free(node->infile);
		node->infile = NULL;
	}
	else if (node->here_doc_fd > -1)
	{
		close(node->here_doc_fd);
		node->here_doc_fd = -2;
	}
	if (redirs->type == HERE_DOC)
		node->here_doc_fd = redirs->here_doc_fd;
	else if (redirs->type == INFILE)
	{
		node->infile = ft_strdup(redirs->file);
		if (!node->infile)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	return (node);
}

int	check_io_files(t_final_cmd_table *node, char *file, int flag)
{
	int		fd;
	char	*tmp;
	char	*cpy;

	fd = -2;
	cpy = ft_strdup(file);
	tmp = remove_quotes_expansion(cpy, quote_removal_str_len(cpy));
	if (flag)
		fd = open(tmp, O_RDONLY);
	else
	{
		if (node->out_type == OUTFILE)
			fd = open(tmp, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (node->out_type == APPEND)
			fd = open(tmp, O_CREAT | O_APPEND | O_WRONLY, 0644);
	}
	free(tmp);
	if (fd == -1)
		return (0);
	if (fd > -1)
		close(fd);
	return (1);
}

t_final_cmd_table	*set_final_redirs(t_minishell *msh,
	t_final_cmd_table *newnode, t_redir_list *redirs)
{
	t_redir_list	*tmp;

	newnode->in_type = NO_REDIR;
	newnode->infile = NULL;
	newnode->infile_fd = -2;
	newnode->here_doc_fd = -2;
	newnode->out_type = NO_REDIR;
	newnode->outfile = NULL;
	newnode->outfile_fd = -2;
	if (!redirs)
		return (newnode);
	tmp = redirs;
	while (tmp)
	{
		newnode = set_redir_info(newnode, tmp);
		if (!newnode)
			return (NULL);
		(void)msh;
		if (newnode->outfile && !check_io_files(newnode, newnode->outfile, 0))
			break ;
		if (newnode->infile && !check_io_files(newnode, newnode->infile, 1))
			break ;
		tmp = tmp->next;
	}
	return (newnode);
}

t_simplecmd	*simplecmdcpy(t_simplecmd *simplecmd)
{
	t_simplecmd	*new_simplecmd;

	new_simplecmd = malloc(sizeof(t_simplecmd));
	if (!new_simplecmd)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_simplecmd->arg_arr = NULL;
	if (simplecmd)
	{
		new_simplecmd->arg_nbr = simplecmd->arg_nbr;
		new_simplecmd->arg_arr = arrdup(simplecmd->arg_arr);
		free_arr(simplecmd->arg_arr);
		if (!new_simplecmd->arg_arr)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		free(simplecmd);
	}
	else
		new_simplecmd->arg_nbr = 0;
	return (new_simplecmd);
}

t_symbol	check_next_symbol(t_command_table *next_node)
{
	if (!next_node)
		return (NO_SYMBOL);
	else if (next_node->type == TABLE_AND)
		return (S_AND);
	else if (next_node->type == TABLE_OR)
		return (S_OR);
	else
		return (S_PIPE);
}
