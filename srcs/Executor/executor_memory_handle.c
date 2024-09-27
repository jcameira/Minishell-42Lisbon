/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_memory_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:37:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/26 21:06:02 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	close_pipes(t_execution_info *info)
{
	if (info->in_pipe[READ] > -1)
		close(info->in_pipe[READ]);
	if (info->in_pipe[WRITE] > -1)
		close(info->in_pipe[WRITE]);
	if (info->out_pipe[READ] > -1)
		close(info->out_pipe[READ]);
	if (info->out_pipe[WRITE] > -1)
		close(info->out_pipe[WRITE]);
}

void	free_f_command_table_node(t_final_cmd_table **cmd_table)
{
	t_final_cmd_table	*tmp;

	free_arr((*cmd_table)->simplecmd->arg_arr);
	free((*cmd_table)->simplecmd);
	if ((*cmd_table)->here_doc_fd > -1)
		close((*cmd_table)->here_doc_fd);
	if ((*cmd_table)->infile)
		free((*cmd_table)->infile);
	if ((*cmd_table)->outfile)
		free((*cmd_table)->outfile);
	tmp = (*cmd_table)->next;
	free(*cmd_table);
	*cmd_table = tmp;
}
