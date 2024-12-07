/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:48:05 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/07 18:22:26 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	mini_pwd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*tmp;

	(void)cmd;
	(void)msh;
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (ft_putstr_fd(PWD_CANNOT_ACCESS_PARENT_DIR, 2), EXIT_FAILURE);
	ft_putendl_fd(tmp, 1);
	free(tmp);
	return (EXIT_SUCCESS);
}
