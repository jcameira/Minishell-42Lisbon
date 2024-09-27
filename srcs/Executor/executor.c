/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 01:05:37 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	execution_setup(t_minishell *msh, t_execution_info *info,
	int *status, int *i)
{
	if (info->tmp_table->builtin && info->pipeline_size == 1)
	{
		if (set_in(info->tmp_table, status) && set_out(info->tmp_table, status))
		{
			if (info->tmp_table->infile_fd > -1)
				dup2(info->tmp_table->infile_fd, STDIN_FILENO);
			if (info->tmp_table->outfile_fd > -1)
				dup2(info->tmp_table->outfile_fd, STDOUT_FILENO);
			*status = info->tmp_table->builtin(msh, info->tmp_table->simplecmd);
			reset_std_fds(msh);
		}
		else
			*status = 1;
	}
	else if (!execute_in_fork(msh, info, i, status))
	{
		close_pipes(info);
		free(info);
		exit_shell(msh, *status);
	}
	if (info->tmp_table->simplecmd->arg_arr
		&& !ft_strcmp(info->tmp_table->simplecmd->arg_arr[0], "exit"))
	{
		if (*status == -1)
			*status = 1;
		free_f_command_table(info->tmp_table);
		free(info->pid);
		close_pipes(info);
		free(info);
		exit_shell(msh, *status);
	}
}

int	next_command_setup(t_execution_info **info, int *status, int *i)
{
	if ((*info)->tmp_table->next_symbol != PIPE)
	{
		close_pipes((*info));
		*i = -1;
		if (!(*info)->tmp_table->builtin || (*info)->pipeline_size > 1)
		{
			while (++(*i) < (*info)->pipeline_size)
				waitpid((*info)->pid[*i], status, 0);
			*status = WEXITSTATUS(*status);
		}
		(*info)->pipeline_start = !(*info)->pipeline_start;
		if (((*info)->tmp_table->next_symbol == AND && *status != 0)
			|| ((*info)->tmp_table->next_symbol == OR && *status == 0))
			return (FAILURE);
	}
	else
	{
		if ((*info)->in_pipe[READ] > -1)
			close((*info)->in_pipe[READ]);
		if ((*info)->in_pipe[WRITE] > -1)
			close((*info)->in_pipe[WRITE]);
		(*info)->in_pipe[WRITE] = (*info)->out_pipe[WRITE];
		(*info)->out_pipe[WRITE] = -1;
		(*info)->in_pipe[READ] = (*info)->out_pipe[READ];
		(*info)->out_pipe[READ] = -1;
	}
	return (SUCCESS);
}

int	executor(t_minishell *msh, t_final_cmd_table *final_cmd_table)
{
	int					i;
	int					status;
	int					skip;
	t_execution_info	*info;

	info = exec_info_init(final_cmd_table);
	if (!info)
		return (EXIT_FAILURE);
	status = 0;
	while (info->tmp_table)
	{
		if (!init_pipeline(&info, &i))
			return (EXIT_FAILURE);
		if (pipe(info->out_pipe) == -1)
			return (free(info), free_f_command_table(info->tmp_table),
				ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
		check_if_pipefd_needed(&info);
		execution_setup(msh, info, &status, &i);
		if (!next_command_setup(&info, &status, &i))
		{
			if (info->tmp_table->next->subshell_level == info->tmp_table->subshell_level)
				free_f_command_table_node(&info->tmp_table);
			else
			{
				skip = info->tmp_table->subshell_level;
				while (info->tmp_table->next && info->tmp_table->next->subshell_level != skip)
					free_f_command_table_node(&info->tmp_table);
			}
		}
		i++;
		free_f_command_table_node(&info->tmp_table);
	}
	free(info->pid);
	free(info);
	return (status);
}
