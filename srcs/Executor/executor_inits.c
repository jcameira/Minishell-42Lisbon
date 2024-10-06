/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_inits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:46:15 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 22:00:29 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

t_execution_info	*exec_info_init(t_final_cmd_table *final_cmd_table)
{
	t_execution_info	*info;

	info = malloc(sizeof(t_execution_info));
	if (!info)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	info->tmp_table = final_cmd_table;
	info->pid = NULL;
	info->in_pipe[READ] = -1;
	info->in_pipe[WRITE] = -1;
	info->out_pipe[READ] = -1;
	info->out_pipe[WRITE] = -1;
	info->descending_subshell_pipe[READ] = -1;
	info->descending_subshell_pipe[WRITE] = -1;
	info->pipeline_start = 1;
	return (info);
}

int	init_pipeline(t_execution_info **info, int *i)
{
	if ((*info)->pipeline_start)
	{
		(*info)->pipeline_size = get_pipeline_size((*info)->tmp_table);
		(*info)->pipeline_start = !(*info)->pipeline_start;
		if ((*info)->pid)
			free((*info)->pid);
		(*info)->pid = malloc(sizeof (pid_t) * (*info)->pipeline_size);
		if (!(*info)->pid)
			return (free((*info)), free_f_command_table((*info)->tmp_table),
				ft_putstr_fd(NO_SPACE, 2), FAILURE);
		*i = 0;
	}
	return (SUCCESS);
}

int	set_info(t_execution_info **info, t_final_cmd_table *final_cmd_table)
{
	if (!(*info))
	{
		(*info) = exec_info_init(final_cmd_table);
		if (!(*info))
			return (FAILURE);
	}
	return (SUCCESS);
}
