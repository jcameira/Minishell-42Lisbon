/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:46 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/13 16:44:28 by mpais-go         ###   ########.fr       */
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
			return ;
	}
}

void	mini_cd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*cur_path;
	char	*final_path;

	cur_path = getcwd(NULL, 0);
	if (cmd->arg_nbr == 1)
	{
		final_path = find_path(msh, cmd, HOME);
		if (!final_path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return ;
		}
	}
	if (cmd->arg_nbr > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (!ft_strncmp(cmd->arg_arr[1], "..", 2))
		final_path = find_slash(cur_path);
	else if (*cmd->arg_arr[1] == '-' )
		final_path = find_path(msh, cmd, OLDPWD);
	else
		final_path = ft_strjoin(cur_path, cmd->arg_arr[1]);
	if (chdir(final_path) == -1)
	{
		ft_putendl_fd("cd: No such file or directory", 1);
		free(final_path);
		free(cur_path);
		return ;
	}
	aux_cd(msh->envp, cur_path, final_path);
}
