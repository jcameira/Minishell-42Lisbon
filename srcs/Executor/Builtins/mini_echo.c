/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:42 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/12 16:40:37 by jcameira         ###   ########.fr       */
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
