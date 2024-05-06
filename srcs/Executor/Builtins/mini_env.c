/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:51 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/06 16:27:18 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_env(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	
	i = -1;
	while (msh->envp[++i])
		ft_putendl_fd(msh->envp[i], 1);
}