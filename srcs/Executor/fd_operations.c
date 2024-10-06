/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:42:55 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 19:52:58 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	close_pipes(t_execution_info *info)
{
	if (info->in_pipe[READ] > -1)
	{
		close(info->in_pipe[READ]);
		info->in_pipe[READ] = -1;
	}
	if (info->in_pipe[WRITE] > -1)
	{
		close(info->in_pipe[WRITE]);
		info->in_pipe[WRITE] = -1;
	}
	if (info->out_pipe[READ] > -1)
	{
		close(info->out_pipe[READ]);
		info->out_pipe[READ] = -1;
	}
	if (info->out_pipe[WRITE] > -1)
	{
		close(info->out_pipe[WRITE]);
		info->out_pipe[WRITE] = -1;
	}
}

void	prepare_next_fds(t_execution_info **info)
{
	if ((*info)->in_pipe[READ] > -1)
		close((*info)->in_pipe[READ]);
	if ((*info)->in_pipe[WRITE] > -1)
		close((*info)->in_pipe[WRITE]);
	(*info)->in_pipe[WRITE] = (*info)->out_pipe[WRITE];
	(*info)->out_pipe[WRITE] = -1;
	(*info)->in_pipe[READ] = (*info)->out_pipe[READ];
	(*info)->out_pipe[READ] = -1;
	if ((*info)->tmp_table->next
		&& (*info)->tmp_table->next->subshell_level \
		> (*info)->tmp_table->subshell_level)
		(*info)->pipeline_start = !(*info)->pipeline_start;
}

void	check_close_io(t_execution_info *info)
{
	if (info->tmp_table->infile_fd > -1
		&& info->tmp_table->infile_fd != info->in_pipe[WRITE]
		&& info->tmp_table->infile_fd != info->in_pipe[READ])
		close (info->tmp_table->infile_fd);
	if (info->tmp_table->outfile_fd > -1
		&& info->tmp_table->outfile_fd != info->in_pipe[WRITE]
		&& info->tmp_table->outfile_fd != info->in_pipe[READ])
		close (info->tmp_table->outfile_fd);
}
