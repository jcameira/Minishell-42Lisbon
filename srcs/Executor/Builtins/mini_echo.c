/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:42 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/05 17:41:03 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	flag_handler(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-' || !str[i + 1])
		return (0);
	while (str[++i])
		if (str[i] != 'n')
			return (0);
	return (1);
}

int	mini_echo(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	int		enter_flag_handler;
	int		flag;

	i = 0;
	flag = 0;
	enter_flag_handler = 1;
	(void)msh;
	while (cmd->arg_arr[++i])
	{
		if (cmd->arg_arr[i][0] != '-')
			enter_flag_handler = !enter_flag_handler;
		if (enter_flag_handler && flag_handler(cmd->arg_arr[i]))
			flag = 1;
		else
		{
			ft_putstr_fd(cmd->arg_arr[i], 1);
			if (cmd->arg_arr[i + 1])
				ft_putstr_fd(" ", 1);
		}
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	return (EXIT_SUCCESS);
}
