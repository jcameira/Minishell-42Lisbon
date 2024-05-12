/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:48:05 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/12 16:03:15 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_pwd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*tmp;

	(void)cmd;
	(void)msh;
	tmp = getcwd(NULL, 0);
	ft_putendl_fd(tmp, 1);
	free(tmp);
	return ;
}
