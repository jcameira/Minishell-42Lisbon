/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_memory_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 00:36:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/10 00:42:54 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	free_redir_list(t_redir_list *redirs)
{
	t_redir_list	*tmp;

	while (redirs)
	{
		if (redirs->file)
			free(redirs->file);
		if (redirs->here_doc_limiter)
			free(redirs->here_doc_limiter);
		if (redirs->here_doc_buffer)
			free(redirs->here_doc_buffer);
		tmp = redirs->next;
		free(redirs);
		redirs = tmp;
	}
}

void	free_f_command_table(t_final_command_table *cmd_table)
{
	t_final_command_table	*tmp;

	while (cmd_table)
	{
		free_arr(cmd_table->simplecmd->arg_arr);
		free(cmd_table->simplecmd);
		if (cmd_table->infile)
			free(cmd_table->infile);
		if (cmd_table->outfile)
			free(cmd_table->outfile);
		if (cmd_table->here_doc_buffer)
			free(cmd_table->here_doc_buffer);
		tmp = cmd_table->next;
		free(cmd_table);
		cmd_table = tmp;
	}
}
