/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:51 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/28 15:32:40 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	mini_env(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	// int		j;

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
	{
		// j = -1;
		// while (msh->envp[i][++j])
		// {
			// if (msh->envp[i][j] == '=')
			// {
			// 	ft_putendl_fd(msh->envp[i], 1);
			// 	break ;
			// }
		// }
		ft_putendl_fd(msh->envp[i], 1);
	}
	return (EXIT_SUCCESS);
}
