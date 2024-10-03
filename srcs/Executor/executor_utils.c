/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:43:57 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:30 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

int	set_in(t_final_cmd_table *final_cmd_table, int *status)
{
	// printf("Infile -> %s\n", final_cmd_table->infile);
	if (final_cmd_table->ambiguous_redirect)
		return (printf(AMBIGUOUS_REDIRECT, final_cmd_table->infile), 0);
	if (final_cmd_table->in_type == INFILE)
		final_cmd_table->infile_fd = open(final_cmd_table->infile,
				O_RDONLY);
	else if (final_cmd_table->in_type == HERE_DOC)
		final_cmd_table->infile_fd = final_cmd_table->here_doc_fd;
	if (final_cmd_table->infile_fd == -1)
	{
		*status = 1;
		return (ft_putstr_fd(ERROR_PREFIX, 2),
		ft_putstr_fd(final_cmd_table->infile, 2),
		ft_putstr_fd(ERROR_NO_FILE, 2), 0);
	}
	*status = 0;
	return (1);
}

int	set_out(t_final_cmd_table *final_cmd_table, int *status)
{
	if (final_cmd_table->ambiguous_redirect)
		return (printf(AMBIGUOUS_REDIRECT, final_cmd_table->outfile), 0);
	if (final_cmd_table->out_type == OUTFILE)
		final_cmd_table->outfile_fd = open(final_cmd_table->outfile,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (final_cmd_table->out_type == APPEND)
		final_cmd_table->outfile_fd = open(final_cmd_table->outfile,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (final_cmd_table->outfile_fd == -1)
	{
		*status = 1;
		return (ft_putstr_fd(ERROR_PREFIX, 2),
		ft_putstr_fd(final_cmd_table->outfile, 2),
		ft_putstr_fd(ERROR_NO_PERMISSION, 2), 0);
	}
	*status = 0;
	return (1);
}

void	reset_std_fds(t_minishell *msh)
{
	if (!isatty(STDIN_FILENO))
		dup2(msh->original_stdin, STDIN_FILENO);
	if (!isatty(STDOUT_FILENO))
		dup2(msh->original_stdout, STDOUT_FILENO);
	if (!isatty(STDERR_FILENO))
		dup2(msh->original_stderr, STDERR_FILENO);
}

int	get_pipeline_size(t_final_cmd_table *final_cmd_table)
{
	int						size;
	t_final_cmd_table		*tmp;

	size = 1;
	tmp = final_cmd_table;
	while (tmp->next_symbol == S_PIPE && tmp->next && tmp->next->subshell_level == tmp->subshell_level)
	{
		tmp = tmp->next;
		size++;
	}
	return (size);
}

void	check_if_pipefd_needed(t_execution_info **info)
{
	if ((*info)->tmp_table->next
		&& (*info)->tmp_table->next->subshell_level < (*info)->tmp_table->subshell_level
		&& (*info)->descending_subshell_pipe[WRITE] > -1
		&& (*info)->descending_subshell_pipe[READ] > -1
		&& (*info)->tmp_table->outfile_fd == -2)
	{
		(*info)->tmp_table->outfile_fd = (*info)->descending_subshell_pipe[WRITE];
		close((*info)->descending_subshell_pipe[READ]);
		(*info)->descending_subshell_pipe[READ] = -1;
	}
	// printf("Read from pipe fd -> %d Write to pipe fd -> %d\n", (*info)->tmp_table->infile_fd, (*info)->tmp_table->outfile_fd);
	if ((*info)->tmp_table->previous_symbol == S_PIPE
		&& (*info)->tmp_table->infile_fd == -2)
		(*info)->tmp_table->infile_fd = (*info)->in_pipe[READ];
	if ((*info)->tmp_table->next_symbol == S_PIPE
		&& (*info)->tmp_table->outfile_fd == -2)
		(*info)->tmp_table->outfile_fd = (*info)->out_pipe[WRITE];
	// printf("Read from pipe fd -> %d Write to pipe fd -> %d\n", (*info)->tmp_table->infile_fd, (*info)->tmp_table->outfile_fd);
}
