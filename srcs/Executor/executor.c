/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/25 23:18:43 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	execution_setup(t_minishell *msh, t_execution_info *info,
	int *status, int *i)
{
	if (info->tmp_table->builtin && info->pipeline_size == 1)
		*status = info->tmp_table->builtin(msh, info->tmp_table->simplecmd);
	else if (!execute_in_fork(msh, info, i))
	{
		close_pipes(info);
		free(info);
		int i = 2;
		while (++(i) < 11)
		if (fcntl(i, F_GETFL) != -1 || errno != EBADF)
			printf("Fd %d is open\n", i);
		exit_shell(msh, FAILURE);
	}
	if (!ft_strcmp(info->tmp_table->simplecmd->arg_arr[0], "exit"))
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
		// *i = 2;
		// while (++(*i) < 11)
		// 	if (fcntl(*i, F_GETFL) != -1 || errno != EBADF)
		// 		printf("Fd %d is open\n", *i);
		// fprintf(stderr, "Is a tty -> %d\n", isatty(1));
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
		printf("Before change %d %d %d %d\n", (*info)->in_pipe[WRITE], (*info)->in_pipe[READ], (*info)->out_pipe[WRITE], (*info)->out_pipe[READ]);
		if ((*info)->in_pipe[READ] > -1 && (*info)->in_pipe[READ] != (*info)->out_pipe[WRITE] && (*info)->in_pipe[READ] != (*info)->out_pipe[READ])
		{
			printf("Closing %d\n", (*info)->in_pipe[READ]);
			close((*info)->in_pipe[READ]);
		}
		if ((*info)->in_pipe[WRITE] > -1 && (*info)->in_pipe[WRITE] != (*info)->out_pipe[WRITE] && (*info)->in_pipe[READ] != (*info)->out_pipe[READ])
		{
			printf("Closing %d\n", (*info)->in_pipe[WRITE]);
			close((*info)->in_pipe[WRITE]);
		}
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
		printf("Next cycle\n");
		if (!init_pipeline(&info, &i))
			return (EXIT_FAILURE);
		// printf("i -> %d size -> %d\n", i, info->pipeline_size);
		// if (i < info->pipeline_size - 1)
		if (pipe(info->out_pipe) == -1)
			return (free(info), free_f_command_table(info->tmp_table),
				ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
		check_if_pipefd_needed(&info);
		printf("Entering execution %d %d %d %d\n", info->in_pipe[WRITE], info->in_pipe[READ], info->out_pipe[WRITE], info->out_pipe[READ]);
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
		printf("%d %d %d %d\n", info->in_pipe[WRITE], info->in_pipe[READ], info->out_pipe[WRITE], info->out_pipe[READ]);
		free_f_command_table_node(&info->tmp_table);
	}
	free(info->pid);
	free(info);
	return (status);
}
