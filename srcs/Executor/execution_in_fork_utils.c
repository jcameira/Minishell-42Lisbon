/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_in_fork_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 23:08:36 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 00:16:37 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <executor.h>

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

int	exec_checks(char *cmd, int *status)
{
	DIR *dir;
	int	fd;

	fd = open(cmd, O_RDONLY);
	if (fd == -1 && errno == EACCES)
	{
		*status = 126;
		return (ft_putstr_fd(ERROR_PREFIX, 2), ft_putstr_fd(cmd, 2),
			ft_putstr_fd(ERROR_NO_PERMISSION, 2), 0);
	}
	if (fd > -1)
		close(fd);
	dir = opendir(cmd);
	if ((!ft_strncmp(cmd, "./", 2) || cmd[0] == '/') && dir)
		*status = 126;
	if ((!ft_strncmp(cmd, "./", 2) || cmd[0] == '/') && !dir)
		*status = 127;
	if (*status == 126)
		return (ft_putstr_fd(ERROR_PREFIX, 2), ft_putstr_fd(cmd, 2),
			ft_putstr_fd(ERROR_DIRECTORY, 2), free(dir), 0);
	if (*status == 127)
		return (ft_putstr_fd(ERROR_PREFIX, 2), ft_putstr_fd(cmd, 2),
			ft_putstr_fd(ERROR_NO_FILE, 2), free(dir), 0);
	return (1);
}

void	get_path(t_minishell *msh, t_final_cmd_table *final_cmd_table,
	char **path)
{
	char	**cmd_paths;

	if (!access(final_cmd_table->simplecmd->arg_arr[0], X_OK)
		&& (!ft_strncmp(final_cmd_table->simplecmd->arg_arr[0], "./", 2)
		|| final_cmd_table->simplecmd->arg_arr[0][0] == '/'))
		*path = final_cmd_table->simplecmd->arg_arr[0];
	else if (msh->private_path)
	{
		cmd_paths = ft_split(msh->private_path, ':');
		if (!cmd_paths)
		{
			ft_putstr_fd(NO_SPACE, 2);
			exit(EXIT_FAILURE);
		}
		*path = write_command_path(cmd_paths,
				final_cmd_table->simplecmd->arg_arr[0]);
	}
	else
		*path = find_cmd_path(msh->envp,
			final_cmd_table->simplecmd->arg_arr[0]);
}
