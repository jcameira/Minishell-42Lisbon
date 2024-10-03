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

int	check_io_files(t_final_cmd_table *node)
{
	int		in_fd;
	int		out_fd;
	char	*tmp;
	char	*cpy;

	in_fd = -2;
	out_fd = -2;
	if (node->infile)
	{
		cpy = ft_strdup(node->infile);
		tmp = remove_quotes_expansion(cpy, quote_removal_str_len(cpy));
		in_fd = open(tmp, O_RDONLY);
		free(tmp);
	}
	if (in_fd == -1)
		return (0);
	if (node->outfile)
	{
		cpy = ft_strdup(node->outfile);
		tmp = remove_quotes_expansion(cpy, quote_removal_str_len(cpy));
		if (node->out_type == OUTFILE)
			out_fd = open(tmp, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (node->out_type == APPEND)
			out_fd = open(tmp, O_CREAT | O_APPEND | O_WRONLY, 0644);
		free(tmp);
	}
	if (out_fd == -1)
	{
		if (in_fd > -1)
			close(in_fd);
		return (0);
	}
	if (in_fd > -1)
		close(in_fd);
	if (out_fd > -1)
		close(out_fd);
	return (1);
}

t_final_cmd_table	*set_final_redirs(t_minishell *msh, t_final_cmd_table *new_table_node,
	t_redir_list *redirs)
{
	t_redir_list	*tmp;

	new_table_node->in_type = NO_REDIR;
	new_table_node->infile = NULL;
	new_table_node->infile_fd = -2;
	new_table_node->here_doc_fd = -2;
	new_table_node->out_type = NO_REDIR;
	new_table_node->outfile = NULL;
	new_table_node->outfile_fd = -2;
	if (!redirs)
		return (new_table_node);
	tmp = redirs;
	while (tmp)
	{
		new_table_node = set_redir_info(new_table_node, tmp);
		if (!new_table_node)
			return (NULL);
		// printf("Infile -> %s\n", new_table_node->infile);
		// printf("Outfile -> %s\n", new_table_node->outfile);
		(void)msh;
		// if (new_table_node->infile)
		// 	new_table_node->ambiguous_redirect = set_ambiguous_redirect(msh, new_table_node, new_table_node->infile);
		// if (new_table_node->outfile)
		// 	new_table_node->ambiguous_redirect = set_ambiguous_redirect(msh, new_table_node, new_table_node->outfile);
		// if (new_table_node->ambiguous_redirect)
		// 	break ;
		if (!check_io_files(new_table_node))
			break ;
		tmp = tmp->next;
	}
	return (new_table_node);
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
