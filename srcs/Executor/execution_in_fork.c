/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_in_fork.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:41:43 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 05:19:26 by jcameira         ###   ########.fr       */
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
			free_arr(cmd_paths);
			return (complete_path);
		}
		free(path);
		free(complete_path);
	}
	free_arr(cmd_paths);
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
	{
		if (!ft_strncmp("PATH", envp[i], 4))
			path = ft_strchr(envp[i], '/');
	}
	if (!path || !path[0])
		return (NULL);
	cmd_paths = ft_split(path, ':');
	if (!cmd_paths)
	{
		ft_putstr_fd(NO_SPACE, 2);
		exit(EXIT_FAILURE);
	}
	return (write_command_path(cmd_paths, cmd));
}

void	execute_cmd(t_minishell *msh, char *path, char **cmd_args,
	t_execution_info *info)
{
	if (!path || access(path, 0))
	{
		close_pipes(info);
		ft_putstr_fd(cmd_args[0], 2);
		ft_putstr_fd(COMMAND_ERROR_MSG, 2);
		free_f_command_table(info->tmp_table);
		free(info->pid);
		free(info);
		exit_shell(msh, COMMAND_NOT_FOUND_CODE);
	}
	if (execve(path, cmd_args, msh->envp) < 0)
	{
		close_pipes(info);
		ft_putstr_fd(EXECUTION_ERROR_MSG_PREFIX, 2);
		perror(cmd_args[0]);
		free_f_command_table(info->tmp_table);
		free(info->pid);
		free(info);
		exit_shell(msh, errno);
	}
}

void	child(t_minishell *msh, t_final_cmd_table *final_cmd_table,
	t_execution_info *info)
{
	char	**cmd_paths;
	char	*path;

	if (final_cmd_table->infile_fd)
		dup2(final_cmd_table->infile_fd, STDIN_FILENO);
	if (final_cmd_table->outfile_fd)
		dup2(final_cmd_table->outfile_fd, STDOUT_FILENO);
	if (!access(final_cmd_table->simplecmd->arg_arr[0], X_OK))
		path = final_cmd_table->simplecmd->arg_arr[0];
	else if (msh->private_path)
	{
		cmd_paths = ft_split(msh->private_path, ':');
		if (!cmd_paths)
		{
			ft_putstr_fd(NO_SPACE, 2);
			exit(EXIT_FAILURE);
		}
		path = write_command_path(cmd_paths,
				final_cmd_table->simplecmd->arg_arr[0]);
	}
	else
		path = find_cmd_path(msh->envp,
				final_cmd_table->simplecmd->arg_arr[0]);
	execute_cmd(msh, path, final_cmd_table->simplecmd->arg_arr, info);
}

int	execute_in_fork(t_minishell *msh, t_execution_info *info, int *i)
{
	ignore_signals_init();
	info->pid[*i] = fork();
	if (info->pid[*i] == 0)
	{
		child_signals_init();
		if (info->tmp_table->builtin && set_in(info->tmp_table)
			&& set_out(info->tmp_table))
		{
			if (info->tmp_table->infile_fd)
				dup2(info->tmp_table->infile_fd, STDIN_FILENO);
			if (info->tmp_table->outfile_fd)
				dup2(info->tmp_table->outfile_fd, STDOUT_FILENO);
			info->tmp_table->builtin(msh, info->tmp_table->simplecmd);
			reset_std_fds(msh);
			return (free_f_command_table(info->tmp_table), free(info->pid),
				FAILURE);
		}
		if (set_in(info->tmp_table) && set_out(info->tmp_table))
			child(msh, info->tmp_table, info);
		else
			return (free_f_command_table(info->tmp_table), free(info->pid),
				FAILURE);
	}
	return (SUCCESS);
}
