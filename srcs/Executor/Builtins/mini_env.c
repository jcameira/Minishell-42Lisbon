/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:51 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/05 17:41:59 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	mini_env(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;

	i = -1;
	(void)cmd;
	if (cmd->arg_nbr > 1)
	{
		ft_putstr_fd(ENV_PREFIX, 2);
		ft_putstr_fd(cmd->arg_arr[1], 2);
		ft_putstr_fd(ENV_FILE_DIR, 2);
		return (EXIT_FAILURE);
	}
	while (msh->envp[++i])
		ft_putendl_fd(msh->envp[i], 1);
	return (EXIT_SUCCESS);
}
