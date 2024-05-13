/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:46:55 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 15:52:35 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	(*buitin_arr(char *command))(t_minishell *msh, t_simplecmd *cmd)
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
			if (!ft_strncmp(builtin[i][0], command, ft_strlen((builtin[i][0]))))
				return (builtin[i][1]);
		}
	}
	return (NULL);
}
