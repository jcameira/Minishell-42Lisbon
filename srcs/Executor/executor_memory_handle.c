/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_memory_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:37:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 06:15:04 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

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

void	execution_info_cleanup(t_minishell *msh, t_execution_info *info,
	int exit_code)
{
	close_pipes(info);
	free_f_command_table(info->tmp_table);
	free(info->pid);
	free(info);
	exit_shell(msh, exit_code);
}

int	skip_executed_commands(t_execution_info *info, t_minishell *msh,
	int status, int level_in_execution)
{
	t_final_cmd_table	*tmp;

	while (info->tmp_table
		&& info->tmp_table->subshell_level > level_in_execution)
	{
		free_f_command_table_node(&info->tmp_table);
		if (info->tmp_table && info->tmp_table->next
			&& info->tmp_table->next->subshell_level == level_in_execution)
		{
			tmp = info->tmp_table;
			logical_operator_skip(info, msh, status, level_in_execution);
			if (info->tmp_table != tmp)
				free_f_command_table_node(&info->tmp_table);
			break ;
		}
	}
	if (!info->tmp_table)
		return (0);
	return (1);
}

void	special_case_exit(t_minishell *msh, t_execution_info *info,
	int *status)
{
	if (info->tmp_table->simplecmd->arg_arr
		&& info->tmp_table->simplecmd->arg_arr[0]
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
