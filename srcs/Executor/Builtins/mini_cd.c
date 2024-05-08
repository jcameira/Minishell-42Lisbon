/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:46 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/08 14:24:26 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cd, cd .. and cd - 
#include <builtins.h>

void	mini_cd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*tmp;
	int		i;
	int		j;
	
	if (cmd->arg_arr[1] == ".." )
		
	if (cmd->arg_arr[1] == "-" )
	{
		i = -1;
		while (msh->envp[++i])
		{
			if (!ft_strncmp(msh->envp[i], OLDPWD, 7))
			{
				tmp = msh->envp[i];
				j = -1;
				while (++j < 7)
					tmp++;
				ft_putendl_fd(tmp, 1);
				break ;
			}
		}
	}
	if (!cmd->arg_arr[1])
	{
		i = -1;
		while (msh->envp[++i])
		{
			if (!ft_strncmp(msh->envp[i], HOME, 5))
			{
				tmp = msh->envp[i];
				j = -1;
				while (++j < 5)
					tmp++;
				ft_putendl_fd(tmp, 1);
				break ;
			}
		}
	}
	else
	{
		// if existe, cenas
		// else
		//ft_putendl_fd("bash: cd: %s: No such file or directory", 1);
	}
}