/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:08:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/03 10:38:59 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	mini_exit(t_minishell *msh, t_simplecmd *cmd)
{
	int	i;
	int	exit_code;

	(void)msh;
	i = -1;
	if (cmd->arg_nbr == 1)
		return (0);
	while (cmd->arg_arr[1][++i])
		if (!ft_isdigit(cmd->arg_arr[1][i]) && cmd->arg_arr[1][i] != '-'
			&& cmd->arg_arr[1][i] != '+')
			return (ft_putstr_fd("exit\n", 1), ft_putstr_fd(EXIT_PREFIX, 2),
				ft_putstr_fd(cmd->arg_arr[1], 2),
				ft_putstr_fd(EXIT_NON_NUMERIC_ARGUMENT, 2), 2);
	if (cmd->arg_nbr > 2)
		return (ft_putstr_fd(EXIT_TOO_MANY_ARGUMENTS, 2), -1);
	exit_code = ft_atoi(cmd->arg_arr[1]);
	return ((unsigned char)exit_code);
}
