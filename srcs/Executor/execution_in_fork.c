/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_in_fork.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:41:43 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 06:29:09 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

void	execute_cmd(t_minishell *msh, char *path, char **cmd_args,
	t_execution_info *info)
{
	if (!path || access(path, F_OK))
	{
		ft_putstr_fd(cmd_args[0], 2);
		ft_putstr_fd(COMMAND_ERROR_MSG, 2);
		free_f_command_table(info->tmp_table);
		free(info->pid);
		free(info);
		exit_shell(msh, COMMAND_NOT_FOUND_CODE);
	}
	if (execve(path, cmd_args, msh->envp) < 0)
	{
		ft_putstr_fd(ERROR_PREFIX, 2);
		perror(cmd_args[0]);
		free_f_command_table(info->tmp_table);
		free(info->pid);
		free(info);
		exit_shell(msh, errno);
	}
}

void	child(t_minishell *msh, t_final_cmd_table *final_cmd_table,
	t_execution_info *info, int *status)
{
	char	*path;

	path = NULL;
	if (final_cmd_table->infile_fd > -1)
		dup2(final_cmd_table->infile_fd, STDIN_FILENO);
	if (final_cmd_table->outfile_fd > -1)
		dup2(final_cmd_table->outfile_fd, STDOUT_FILENO);
	close_pipes(info);
	if (!final_cmd_table->simplecmd->arg_arr
		|| !(*final_cmd_table->simplecmd->arg_arr)
		|| !exec_checks(*final_cmd_table->simplecmd->arg_arr, status))
	{
		free_f_command_table(info->tmp_table);
		free(info->pid);
		free(info);
		exit_shell(msh, *status);
	}
	get_path(msh, final_cmd_table, &path);
	execute_cmd(msh, path, final_cmd_table->simplecmd->arg_arr, info);
}

int	execute_in_fork(t_minishell *msh, t_execution_info *info, int *i,
	int *status)
{
	info->pid[*i] = fork();
	if (info->pid[*i] == 0)
	{
		*status = 0;
		if (info->tmp_table->builtin && set_in(info->tmp_table, status)
			&& set_out(info->tmp_table, status))
		{
			if (info->tmp_table->infile_fd > -1)
				dup2(info->tmp_table->infile_fd, STDIN_FILENO);
			if (info->tmp_table->outfile_fd > -1)
				dup2(info->tmp_table->outfile_fd, STDOUT_FILENO);
			*status = info->tmp_table->builtin(msh, info->tmp_table->simplecmd);
			return (reset_std_fds(msh), free_f_command_table(info->tmp_table),
				free(info->pid), FAILURE);
		}
		if (*status)
			return (free_f_command_table(info->tmp_table), free(info->pid),
				FAILURE);
		if (set_in(info->tmp_table, status) && set_out(info->tmp_table, status))
			child(msh, info->tmp_table, info, status);
		else
			return (free_f_command_table(info->tmp_table), free(info->pid),
				FAILURE);
	}
	return (SUCCESS);
}
