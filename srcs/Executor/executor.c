/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/11 15:26:13 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>

char	*write_command_path(char **cmd_paths, char *cmd)
{
	int		i;
	char	*path;
	char	*complete_path;

	i = -1;
	while (cmd_paths[++i])
	{
		path = ft_strjoin(cmd_paths[i], "/");
		complete_path = ft_strjoin(path, cmd);
		if (!access(complete_path, 0))
		{
			//free_cmds(cmd_paths);
			return (complete_path);
		}
		free(path);
		free(complete_path);
	}
	//free_cmds(cmd_paths);
	if (!access(cmd, 0))
		return (cmd);
	return (NULL);
}

char	*find_cmd_path(char **envp, char *cmd)
{
	int		i;
	char	*path;
	char	**cmd_paths;

	path = NULL;
	i = -1;
	while (envp[++i])
		if (!ft_strncmp("PATH", envp[i], 4))
			path = ft_strchr(envp[i], '/');
	if (!path || !path[0])
		return (NULL);
	cmd_paths = ft_split(path, ':');
	if (!cmd_paths)
		exit(EXIT_FAILURE);
	return (write_command_path(cmd_paths, cmd));
}

void	execute_cmd_bonus(char *path, char **cmd_args, char **envp)
{
	if (!path || access(path, 0))
	{
		//close_everything_bonus(info);
		//write_command_error_bonus(info, cmd_args);
	}
	if (execve(path, cmd_args, envp) < 0)
	{
		//execute_error_bonus(info, cmd_args);
		exit(errno);
	}
}

void	child(t_minishell *msh, t_final_command_table *final_command_table)
{
	char	*path;

	if (final_command_table->infile_fd)
		dup2(final_command_table->infile_fd, STDIN_FD);
	if (final_command_table->outfile_fd)
		dup2(final_command_table->outfile_fd, STDOUT_FD);
	if (!access(final_command_table->simplecmd->arg_arr[0], X_OK))
		path = final_command_table->simplecmd->arg_arr[0];
	else if (!msh->envp)
		path = ft_strjoin(DEFAULT_CMD_PATH,
				final_command_table->simplecmd->arg_arr[0]);
	else
		path = find_cmd_path(msh->envp,
				final_command_table->simplecmd->arg_arr[0]);
	execute_cmd_bonus(path, final_command_table->simplecmd->arg_arr, msh->envp);
}

void	executor_simplecommand(t_minishell *msh,
	t_final_command_table *final_command_table, pid_t *pid, int *i)
{
	if (final_command_table->builtin)
	{
		final_command_table->builtin(msh, final_command_table->simplecmd);
		return ;
	}
	pid[*i] = fork();
	if (pid[*i] == 0)
		child(msh, final_command_table);
}

int	set_in(t_final_command_table *final_command_table)
{
	if (final_command_table->in_type == NO_REDIR)
		final_command_table->infile_fd = -2;
	else if (final_command_table->in_type == INFILE)
		final_command_table->infile_fd = open(final_command_table->infile,
				O_RDONLY);
	else if (final_command_table->in_type == HERE_DOC)
		final_command_table->infile_fd = final_command_table->here_doc_fd;
	if (final_command_table->infile_fd == -1)
		return (ft_putstr_fd(OPEN_IN_ERROR, 2), 0);
	return (1);
}

int	set_out(t_final_command_table *final_command_table)
{
	if (final_command_table->out_type == NO_REDIR)
		final_command_table->outfile_fd = -2;
	else if (final_command_table->out_type == OUTFILE)
		final_command_table->outfile_fd = open(final_command_table->outfile,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (final_command_table->out_type == APPEND)
		final_command_table->outfile_fd = open(final_command_table->outfile,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (final_command_table->outfile_fd == -1)
		return (ft_putstr_fd(OPEN_OUT_ERROR, 2), 0);
	return (1);
}

int	get_pipeline_size(t_final_command_table *final_command_table)
{
	int						size;
	t_final_command_table	*tmp;

	size = 1;
	tmp = final_command_table;
	while (tmp->next_symbol == PIPE)
	{
		tmp = tmp->next;
		size++;
	}
	return (size);
}

int	executor(t_minishell *msh, t_final_command_table *final_command_table)
{
	int						pipeline_start;
	int						pipeline_size;
	pid_t					*pid;
	int						in_pipe[2];
	int						out_pipe[2];
	int 					i;
	int 					status;
	t_final_command_table	*tmp;

	if (pipe(in_pipe) || pipe(out_pipe) == -1)
	{
		free_f_command_table(final_command_table);
		return (ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
	}
	pipeline_start = 1;
	tmp = final_command_table;
	while (tmp)
	{
		if (pipeline_start)
		{
			pipeline_size = get_pipeline_size(tmp);
			printf("Pipeline size -> %d\n", pipeline_size);
			pipeline_start = !pipeline_start;
			pid = malloc(sizeof (pid_t) * pipeline_size);
			if (!pid)
				return (EXIT_FAILURE);
			i = 0;
		}
		if (!set_in(tmp) || !set_out(tmp))
			break ;
		if (tmp->previous_symbol == PIPE
			&& tmp->infile_fd == -2)
			tmp->infile_fd = in_pipe[READ];
		if (tmp->next_symbol == PIPE
			&& tmp->outfile_fd == -2)
			tmp->outfile_fd = out_pipe[WRITE];
		executor_simplecommand(msh, tmp, pid, &i);
		if (tmp->next_symbol != PIPE)
		{
			if (in_pipe[READ])
				close(in_pipe[READ]);
			if (in_pipe[WRITE])
				close(in_pipe[WRITE]);
			if (out_pipe[READ])
				close(out_pipe[READ]);
			if (out_pipe[WRITE])
				close(out_pipe[WRITE]);
			i = -1;
			while (++i < pipeline_size)
				waitpid(pid[i], &status, 0);
			pipeline_start = !pipeline_start;
			tmp = tmp->next;
			continue ;
		}
		else
		{
			if (in_pipe[READ])
				close(in_pipe[READ]);
			if (in_pipe[WRITE])
				close(in_pipe[WRITE]);
			in_pipe[WRITE] = out_pipe[WRITE];
			in_pipe[READ] = out_pipe[READ];
		}
		if (pipe(out_pipe) == -1)
		{
			free_f_command_table(tmp);
			return (ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
		}
		i++;
		tmp = tmp->next;
	}
	fprintf(stderr, "Here\n");
	free(pid);
	free_f_command_table(final_command_table);
	//free_everything_bonus(&info);
	return (WEXITSTATUS(status));
}
