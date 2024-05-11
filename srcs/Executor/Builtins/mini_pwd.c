/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:48:05 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/11 14:07:57 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	mini_pwd(t_minishell *msh, t_simplecmd *cmd)
{
	char	*tmp;

	(void)cmd;
	tmp = getcwd(NULL, 0);
	ft_putendl_fd(tmp, 1);
	free(tmp);
	return (1);
}
