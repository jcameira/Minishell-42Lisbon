/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 00:12:13 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	execute_setup(t_minishell *msh, t_execution_info *info,
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
			if (!ft_strcmp(info->tmp_table->simplecmd->arg_arr[0], "exit")
				&& info->tmp_table->subshell_level == 0)
				ft_putstr_fd("exit\n", 2);
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
	special_case_exit(msh, info, status);
}

int	next_command_setup(t_minishell *msh, t_execution_info **info, int *status,
	int *i)
{
	if ((*info)->tmp_table->next_symbol != S_PIPE || ((*info)->tmp_table->next
			&& (*info)->tmp_table->next->subshell_level \
		< (*info)->tmp_table->subshell_level))
	{
		close_pipes((*info));
		*i = -1;
		if (!(*info)->tmp_table->builtin || (*info)->pipeline_size > 1)
		{
			while (++(*i) < (*info)->pipeline_size)
				waitpid((*info)->pid[*i], status, 0);
			*status = WEXITSTATUS(*status);
		}
		msh->exit_code = *status;
		(*info)->pipeline_start = !(*info)->pipeline_start;
		if (((*info)->tmp_table->next_symbol == S_AND && *status != 0)
			|| ((*info)->tmp_table->next_symbol == S_OR && *status == 0)
			|| ((*info)->tmp_table->next
				&& (*info)->tmp_table->next->subshell_level \
			< (*info)->tmp_table->subshell_level)
			|| (!(*info)->tmp_table->next
				&& (*info)->tmp_table->subshell_level > 0))
			return (FAILURE);
	}
	return (prepare_next_fds(info), SUCCESS);
}

void	logical_operator_skip(t_execution_info *info, t_minishell *msh,
	int status, int level_in_execution)
{
	int	skip;

	check_if_exit_process_needed(info, msh, status, level_in_execution);
	while (info->tmp_table->next_symbol != NO_SYMBOL)
	{
		if (info->tmp_table->next->subshell_level \
		> info->tmp_table->subshell_level)
		{
			skip = info->tmp_table->subshell_level;
			while (info->tmp_table->next
				&& info->tmp_table->next->subshell_level > skip)
				free_f_command_table_node(&info->tmp_table);
			continue ;
		}
		while (info->tmp_table->next_symbol != S_AND
			&& info->tmp_table->next_symbol != S_OR
			&& info->tmp_table->next_symbol != NO_SYMBOL)
			free_f_command_table_node(&info->tmp_table);
		if ((info->tmp_table->next_symbol == S_AND && status != 0)
			|| (info->tmp_table->next_symbol == S_OR && status == 0))
			free_f_command_table_node(&info->tmp_table);
		else
			break ;
	}
}

int	prepare_and_execute_cmd(t_execution_info *info, t_minishell *msh,
	int *status, int level_in_execution)
{
	static int	i;

	if (!init_pipeline(&info, &i))
		return (FAILURE);
	if (pipe(info->out_pipe) == -1)
		return (free(info), free_f_command_table(info->tmp_table),
			ft_putstr_fd(OPEN_PIPE_ERROR, 2), FAILURE);
	check_if_pipefd_needed(&info);
	expander(msh, info->tmp_table);
	execute_setup(msh, info, status, &i);
	if (!next_command_setup(msh, &info, status, &i))
		logical_operator_skip(info, msh, *status, level_in_execution);
	i++;
	return (SUCCESS);
}

int	executor(t_execution_info *info, t_minishell *msh,
	t_final_cmd_table *final_cmd_table, int level_in_execution)
{
	int			status;

	if (!set_info(&info, final_cmd_table))
		return (EXIT_FAILURE);
	status = 0;
	child_signals_init();
	while (info->tmp_table)
	{
		if (info->tmp_table->subshell_level > level_in_execution)
			if (!subshell_fork_exec(info, msh, status, level_in_execution))
				return (EXIT_FAILURE);
		if (!skip_executed_commands(info, level_in_execution))
			break ;
		if (!prepare_and_execute_cmd(info, msh, &status, level_in_execution))
			return (EXIT_FAILURE);
		check_close_io(info);
		free_f_command_table_node(&info->tmp_table);
	}
	free(info->pid);
	free(info);
	if (level_in_execution > 0)
		exit_shell(msh, msh->exit_code);
	return (status);
}
