/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_memory_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 00:36:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/26 21:06:02 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	free_symbol_node(t_command_table **command_table)
{
	t_command_table	*tmp;

	tmp = (*command_table)->next;
	free_redir_list((*command_table)->redirs, 0);
	if ((*command_table)->simplecmd)
	{
		free_arr((*command_table)->simplecmd->arg_arr);
		free((*command_table)->simplecmd);
	}
	free(*command_table);
	*command_table = tmp;
}

void	free_redir_list(t_redir_list *redirs, int close_all_fds)
{
	t_redir_list	*tmp;

	while (redirs)
	{
		if (redirs->file)
			free(redirs->file);
		if (redirs->here_doc_limiter)
			free(redirs->here_doc_limiter);
		if (close_all_fds && redirs->here_doc_fd > -1)
			close(redirs->here_doc_fd);
		tmp = redirs->next;
		free(redirs);
		redirs = tmp;
	}
}

void	free_f_command_table(t_final_cmd_table *cmd_table)
{
	t_final_cmd_table	*tmp;

	while (cmd_table)
	{
		free_arr(cmd_table->simplecmd->arg_arr);
		free(cmd_table->simplecmd);
		if (cmd_table->infile)
			free(cmd_table->infile);
		if (cmd_table->outfile)
			free(cmd_table->outfile);
		if (cmd_table->infile_fd > -1)
			close(cmd_table->infile_fd);
		if (cmd_table->outfile_fd > -1)
			close(cmd_table->outfile_fd);
		if (cmd_table->here_doc_fd > -1)
			close(cmd_table->here_doc_fd);
		tmp = cmd_table->next;
		free(cmd_table);
		cmd_table = tmp;
	}
}
