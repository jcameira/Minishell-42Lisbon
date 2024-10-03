/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/03 10:38:55 by jcameira         ###   ########.fr       */
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
			if (!ft_strcmp(info->tmp_table->simplecmd->arg_arr[0], "exit") && info->tmp_table->subshell_level == 0)
				ft_putstr_fd("exit\n", 1);
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
	if (info->tmp_table->simplecmd->arg_arr && info->tmp_table->simplecmd->arg_arr[0]
		&& !ft_strcmp(info->tmp_table->simplecmd->arg_arr[0], "exit"))
	{
		if (*status == -1)
		{
			*status = 1;
			return ;
		}
		free_f_command_table(info->tmp_table);
		free(info->pid);
		close_pipes(info);
		free(info);
		exit_shell(msh, *status);
	}
}

int	next_command_setup(t_minishell *msh, t_execution_info **info, int *status, int *i)
{
	if ((*info)->tmp_table->next_symbol != S_PIPE || ((*info)->tmp_table->next && (*info)->tmp_table->next->subshell_level < (*info)->tmp_table->subshell_level))
	{
		close_pipes((*info));
		*i = -1;
		if (!(*info)->tmp_table->builtin || (*info)->pipeline_size > 1)
		{
			while (++(*i) < (*info)->pipeline_size)
				waitpid((*info)->pid[*i], status, 0);
			*status = WEXITSTATUS(*status);
		}
		// printf("Exit_code -> %d\n", *status);
		msh->exit_code = *status;
		(*info)->pipeline_start = !(*info)->pipeline_start;
		if (((*info)->tmp_table->next_symbol == S_AND && *status != 0)
			|| ((*info)->tmp_table->next_symbol == S_OR && *status == 0)
			|| ((*info)->tmp_table->next && (*info)->tmp_table->next->subshell_level < (*info)->tmp_table->subshell_level)
			|| (!(*info)->tmp_table->next && (*info)->tmp_table->subshell_level > 0))
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
		if ((*info)->tmp_table->next && (*info)->tmp_table->next->subshell_level > (*info)->tmp_table->subshell_level)
			(*info)->pipeline_start = !(*info)->pipeline_start;
	}
	return (SUCCESS);
}

int	executor(t_execution_info *info, t_minishell *msh, t_final_cmd_table *final_cmd_table, int level_in_execution)
{
	static int	i;
	int			status;
	int			skip;
	pid_t		subshell;

	if (!info)
	{
		info = exec_info_init(final_cmd_table);
		if (!info)
			return (EXIT_FAILURE);
	}
	status = 0;
	while (info->tmp_table)
	{
		if (info->tmp_table->subshell_level > level_in_execution)
		{
			subshell = fork();
			if (subshell == 0)
			{
				// printf("Next subshell\n");
				executor(info, msh, final_cmd_table, ++level_in_execution);
			}
			close_pipes(info);
			// printf("Waiting for subshell level %d to finish executing\n", level_in_execution + 1);
			waitpid(subshell, &status, 0);
			// printf("Finished waiting for subshell level %d to finish executing\n", level_in_execution + 1);
			msh->exit_code = WEXITSTATUS(status);
			// printf("Command to be executed -> %s Exit code -> %d\n", info->tmp_table->simplecmd->arg_arr[0], msh->exit_code);
			// if (((info->tmp_table->next && info->tmp_table->next->subshell_level < level_in_execution) || (!info->tmp_table->next && info->tmp_table->subshell_level > 0)))
			if (info->tmp_table->next && info->tmp_table->next->subshell_level < level_in_execution)
			{
				// printf("Exiting subshell level %d\n", level_in_execution);
				close_pipes(info);
				free_f_command_table(info->tmp_table);
				free(info->pid);
				free(info);
				exit_shell(msh, msh->exit_code);
			}
		}
		while (info->tmp_table && info->tmp_table->subshell_level > level_in_execution)
			free_f_command_table_node(&info->tmp_table);
		if (!info->tmp_table)
			break ;
		// printf("Executing subshell level %d\n", level_in_execution);
		// printf("Command to be executed -> %s Exit code -> %d\n", info->tmp_table->simplecmd->arg_arr[0], status);
		if (!init_pipeline(&info, &i))
			return (EXIT_FAILURE);
		if (pipe(info->out_pipe) == -1)
			return (free(info), free_f_command_table(info->tmp_table),
				ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
		// printf("Pipeline size -> %d On subshell level -> %d Command with index %d\n", info->pipeline_size, info->tmp_table->subshell_level, i);
		check_if_pipefd_needed(&info);
		// printf("Command to be executed -> %s With index %d\n", info->tmp_table->simplecmd->arg_arr[0], i);
		expander(msh, info->tmp_table);
		// printf("Command to be executed -> %s With index %d\n", info->tmp_table->simplecmd->arg_arr[0], i);
		// status = 0;
		execution_setup(msh, info, &status, &i);
		// printf("Command to be executed -> %s With index %d\n", info->tmp_table->simplecmd->arg_arr[0], i);
		if (!next_command_setup(msh, &info, &status, &i))
		{
			if ((info->tmp_table->next && info->tmp_table->next->subshell_level < level_in_execution) || (!info->tmp_table->next && info->tmp_table->subshell_level > 0))
			{
				close_pipes(info);
				free_f_command_table(info->tmp_table);
				free(info->pid);
				free(info);
				exit_shell(msh, status);
			}
			if (info->tmp_table->next->subshell_level == info->tmp_table->subshell_level)
			{
				while (info->tmp_table->next_symbol != NO_SYMBOL)
				{
					while (info->tmp_table->next_symbol != S_AND && info->tmp_table->next_symbol != S_OR && info->tmp_table->next_symbol != NO_SYMBOL)
						free_f_command_table_node(&info->tmp_table);
					if ((info->tmp_table->next_symbol == S_AND && status != 0) || (info->tmp_table->next_symbol == S_OR && status == 0))
						free_f_command_table_node(&info->tmp_table);
					else
						break ;
				}
			}
			else
			{
				skip = info->tmp_table->subshell_level;
				while (info->tmp_table->next && info->tmp_table->next->subshell_level != skip)
					free_f_command_table_node(&info->tmp_table);
			}
			while ((info->tmp_table->next_symbol == S_AND && status != 0) || (info->tmp_table->next_symbol == S_OR && status == 0))
			{
				free_f_command_table_node(&info->tmp_table);
			}
		}
		i++;
		if (info->tmp_table->infile_fd > -1
			&& info->tmp_table->infile_fd != info->in_pipe[WRITE]
			&& info->tmp_table->infile_fd != info->in_pipe[READ])
			close (info->tmp_table->infile_fd);
		if (info->tmp_table->outfile_fd > -1
			&& info->tmp_table->outfile_fd != info->in_pipe[WRITE]
			&& info->tmp_table->outfile_fd != info->in_pipe[READ])
			close (info->tmp_table->outfile_fd);
		free_f_command_table_node(&info->tmp_table);
	}
	free(info->pid);
	free(info);
	return (status);
}
