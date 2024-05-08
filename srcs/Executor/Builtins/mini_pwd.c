/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:48:05 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/08 14:22:16 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>


void	mini_pwd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*tmp;
	int		i;
	int		j;
	
	i = -1;
	(void)cmd;
	while (msh->envp[++i])
	{
		if (!ft_strncmp(msh->envp[i], PWD, 4))
		{
			tmp = msh->envp[i];
			j = -1;
			while (++j < 4)
				tmp++;
			ft_putendl_fd(tmp, 1);
			break ;
		}
	}
}