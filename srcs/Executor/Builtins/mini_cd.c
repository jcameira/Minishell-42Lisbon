/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:46 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/24 23:06:08 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	update_envs(char **msh_envp, char *cur_path, char *final_path)
{
	int	j;
	int	flag;

	j = -1;
	flag = 0;
	while (msh_envp[++j])
	{
		if (!ft_strncmp(msh_envp[j], PWD, 4))
		{
			free(msh_envp[j]);
			msh_envp[j] = ft_strjoin(PWD, final_path);
			flag++;
		}
		if (!ft_strncmp(msh_envp[j], OLDPWD, 7))
		{
			free(msh_envp[j]);
			msh_envp[j] = ft_strjoin(OLDPWD, cur_path);
			flag++;
		}
		if (flag == 2)
			break ;
	}
	free(final_path);
	free(cur_path);
}

int	chdir_cd(char *cur_path, char *final_path, t_simplecmd *cmd)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (strcmp(final_path, tmp) && chdir(final_path) == -1)
	{
		free(final_path);
		free(cur_path);
		free(tmp);
		return (ft_putstr_fd(CD_PREFIX, 2), ft_putstr_fd(cmd->arg_arr[1], 2), ft_putstr_fd(CD_NO_FILE_DIR, 2), 0);
	}
	free(tmp);
	return (1);
}

int	cd_special_checks(t_minishell *msh, t_simplecmd *cmd, char **final_path,
	char *cur_path)
{
	*final_path = NULL;
	if (cmd->arg_nbr > 2)
		return (ft_putstr_fd(CD_TOO_MANY_ARGS, 2), FAILURE);
	else if (cmd->arg_nbr == 1 || (cmd->arg_nbr == 2
			&& *cmd->arg_arr[1] == '~'))
	{
		*final_path = find_path(msh, HOME);
		if (!(*final_path))
			return (ft_putstr_fd(CD_NO_HOME, 2), FAILURE);
	}
	else if (cmd->arg_nbr > 1 && !ft_strncmp(cmd->arg_arr[1], "..", 2))
		*final_path = parent_dir(cur_path);
	else if (!ft_strcmp(cmd->arg_arr[1], "-"))
	{
		*final_path = find_path(msh, OLDPWD);
		if (!(*final_path))
			return (ft_putstr_fd(CD_NO_OLDPWD, 2), FAILURE);
	}
	else if (cmd->arg_arr[1][0] == '/')
	{
		*final_path = ft_strdup(cur_path);
		if (!(*final_path))
			return (ft_putstr_fd(NO_SPACE, 2), FAILURE);
	}
	return (SUCCESS);
}

int	mini_cd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*cur_path;
	char	*tmp;
	char	*final_path;

	cur_path = getcwd(NULL, 0);
	if (!cd_special_checks(msh, cmd, &final_path, cur_path))
		return (EXIT_FAILURE);
	if (!final_path)
	{
		if (cur_path[ft_strlen(cur_path) - 1] != '/')
		{
			tmp = ft_strjoin(cur_path, "/");
			if (!tmp)
				return (ft_putstr_fd(NO_SPACE, 2), EXIT_FAILURE);
			final_path = ft_strjoin(tmp, cmd->arg_arr[1]);
			free(tmp);
		}
		else
			final_path = ft_strjoin(cur_path, cmd->arg_arr[1]);
		if (!final_path)
			return (ft_putstr_fd(NO_SPACE, 2), EXIT_FAILURE);
	}
	if (!chdir_cd(cur_path, final_path, cmd))
		return (EXIT_FAILURE);
	return (update_envs(msh->envp, cur_path, final_path), EXIT_SUCCESS);
}
