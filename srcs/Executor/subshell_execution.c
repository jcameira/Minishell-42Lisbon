/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:32:16 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 05:35:33 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	check_if_exit_process_needed(t_execution_info *info, t_minishell *msh,
	int status, int level_in_execution)
{
	if ((info->tmp_table->next
			&& info->tmp_table->next->subshell_level < level_in_execution)
		|| (!info->tmp_table->next && info->tmp_table->subshell_level > 0))
	{
		if (info->descending_subshell_pipe[WRITE] > -1)
		{
			close(info->descending_subshell_pipe[WRITE]);
			info->descending_subshell_pipe[WRITE] = -1;
		}
		execution_info_cleanup(msh, info, status);
	}
}

int	end_of_level_pipe(t_execution_info *info, int level_in_execution)
{
	t_final_cmd_table	*tmp;

	tmp = info->tmp_table;
	while (tmp->next)
	{
		if (tmp->next->subshell_level < tmp->subshell_level
			&& tmp->next->subshell_level == level_in_execution
			&& tmp->next_symbol == S_PIPE)
		{
			if (pipe(info->descending_subshell_pipe) == -1)
				return (ft_putstr_fd(OPEN_PIPE_ERROR, 2), FAILURE);
			break ;
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int	subshell_fork_exec(t_execution_info *info, t_minishell *msh,
	int *status, int level_in_execution)
{
	pid_t	subshell;

	if (!end_of_level_pipe(info, level_in_execution))
		return (free(info), free_f_command_table(info->tmp_table),
			FAILURE);
	subshell = fork();
	if (subshell == 0)
		executor(info, msh, info->tmp_table, ++level_in_execution);
	close_pipes(info);
	waitpid(subshell, status, 0);
	*status = WEXITSTATUS(*status);
	msh->exit_code = *status;
	if (info->tmp_table->next
		&& info->tmp_table->next->subshell_level < level_in_execution)
		execution_info_cleanup(msh, info, msh->exit_code);
	if (info->descending_subshell_pipe[WRITE] > -1
		&& info->descending_subshell_pipe[WRITE] > -1)
	{
		info->in_pipe[WRITE] = info->descending_subshell_pipe[WRITE];
		info->descending_subshell_pipe[WRITE] = -1;
		info->in_pipe[READ] = info->descending_subshell_pipe[READ];
		info->descending_subshell_pipe[READ] = -1;
	}
	return (SUCCESS);
}
