/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:08:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 23:48:31 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int mini_exit(t_minishell *msh, t_simplecmd *cmd)
{
    int exit_code;

    (void)msh;
    if (cmd->arg_nbr > 2)
    {
        ft_putstr_fd(EXIT_PREFIX, 2);
        ft_putstr_fd(EXIT_TOO_MANY_ARGUMENTS, 2);
    }
    exit_code = ft_atoi(cmd->arg_arr[1]);
	return (exit_code);
}