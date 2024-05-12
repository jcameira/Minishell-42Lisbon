/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:51 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/12 16:00:38 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_env(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	int		j;

	i = -1;
	(void)cmd;
	while (msh->envp[++i])
	{
		j = -1;
		while (msh->envp[i][++j])
		{
			if (msh->envp[i][j] == '=')
			{
				ft_putendl_fd(msh->envp[i], 1);
				break ;
			}
		}
	}
}