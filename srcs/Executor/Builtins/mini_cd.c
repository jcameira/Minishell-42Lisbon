/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:46 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/17 16:52:19 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	aux_cd(char **msh_envp, char *cur_path, char *final_path)
{
	int	j;
	int	flag;

	j = -1;
	flag = 0;
	while (msh_envp[++j])
	{
		if (!ft_strncmp(msh_envp[++j], PWD, 4))
		{
			free(msh_envp[j]);
			msh_envp[j] = ft_strjoin(PWD, final_path);
			flag++;
		}
		if (!ft_strncmp(msh_envp[++j], OLDPWD, 7))
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

int	chdir_cd(char *cur_path, char *final_path)
{
	if (chdir(final_path) == -1)
	{
		free(final_path);
		free(cur_path);
		return (ft_putendl_fd("cd: No such file or directory", 1), 0);
	}
	return (1);
}

void	mini_cd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*cur_path;
	char	*tmp;
	char	*final_path;

	cur_path = getcwd(NULL, 0);
	if (cmd->arg_nbr > 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2));
	if (cmd->arg_nbr == 1)
	{
		final_path = find_path(msh, cmd, HOME);
		if (!final_path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2));
	}
	else if (!ft_strncmp(cmd->arg_arr[1], "..", 2))
		final_path = find_last_slash(cur_path);
	else if (*cmd->arg_arr[1] == '-' )
	{
		final_path = find_path(msh, cmd, OLDPWD);
		if (!final_path)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2));
	}
	else
	{
		if (cur_path[ft_strlen(cur_path) - 1] != '/')
		{
			tmp = ft_strjoin(cur_path, "/");
			if (!tmp)
				return (ft_putstr_fd(NO_SPACE, 2));
			final_path = ft_strjoin(tmp, cmd->arg_arr[1]);
			free(tmp);
			if (!final_path)
				return (ft_putstr_fd(NO_SPACE, 2));
		}
		else
		{
			final_path = ft_strjoin(cur_path, cmd->arg_arr[1]);
			if (!final_path)
				return (ft_putstr_fd(NO_SPACE, 2));
		}
	}
	if (!chdir_cd(cur_path, final_path))
		return ;
	aux_cd(msh->envp, cur_path, final_path);
}
