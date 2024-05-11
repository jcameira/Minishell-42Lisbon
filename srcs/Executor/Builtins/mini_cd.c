/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:46 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/11 16:17:14 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	*find_slash(char *path)
{
	int	i;
	int	j;
	char *new_path;

	i = -1;
	j = 0;
	while(path[++i])
	{
		if (path[i] == "/")
			j = i;
	}
	i = 0;
	new_path = malloc(sizeof (char) * (j + 1));
	while (i < j)
	{
		new_path[i] = path[i];
	}
	new_path[i] = NULL;
	free(path);
	return(new_path);
}

void	mini_cd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*cur_path;
	char	*final_path;
	int		i;
	int		j;
	int		flag;
	
	flag = 0;
	cur_path = getcwd(NULL, 0);
	if (cmd->arg_nbr == 1)
	{
		final_path = find_path(msh, cmd, HOME);
		if (!final_path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			free(final_path);
			return ;
		}
	}
	if (cmd->arg_nbr > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return;
	}
	if (cmd->arg_arr[1] == '..' )
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
		return (1);
	}
	while (msh->envp[++j])
	{
		if (!ft_strncmp(msh->envp[++j], PWD, 4))
		{
			free(msh->envp[i]);
			msh->envp[i] = ft_strjoin(PWD, final_path);
			flag++;
		}
		if (!ft_strncmp(msh->envp[++j], OLDPWD, 7))
		{
			free(msh->envp[i]);
			msh->envp[i] = ft_strjoin(OLDPWD, cur_path);
			flag++;
		}
		if (flag == 2)
			return ;
	}
}