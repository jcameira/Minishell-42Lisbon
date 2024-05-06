/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:42 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/06 20:58:27 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_echo(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	
	i = 0;
	(void)msh;
	while (cmd->arg_arr[++i])
		ft_putendl_fd(cmd->arg_arr[i], 1);
}