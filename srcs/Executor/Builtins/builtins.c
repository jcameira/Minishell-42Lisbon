/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:46:55 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/19 20:29:35 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	(*builtin_arr(char *command))(t_minishell *msh, t_simplecmd *cmd)
{
	static void	*builtin[7][2] = {
	{"cd", mini_cd},
	{"echo", mini_echo},
	{"pwd", mini_pwd},
	{"export", mini_export},
	{"unset", mini_unset},
	{"env", mini_env},
	{"exit", exit_shell}
	};
	int			i;

	i = -1;
	while (++i < 7)
	{
		if (command)
		{
			if (!ft_strcmp(builtin[i][0], command))
				return (builtin[i][1]);
		}
	}
	return (NULL);
}
