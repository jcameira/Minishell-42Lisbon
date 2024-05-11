/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:42 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/10 16:45:20 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_echo(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	
	i = 0;
	(void)msh;
	while (cmd->arg_arr[++i])
	{
		ft_putstr_fd(cmd->arg_arr[i], 1);
		if (cmd->arg_arr[i + 1])
			ft_putstr_fd(" ", 1);
	}
	ft_putstr_fd("\n", 1);
}