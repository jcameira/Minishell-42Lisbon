/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:12:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/01 02:14:59 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	minishell_init(t_minishell *msh, char **envp)
{
	msh->envp = arrdup(envp);
	if (!msh->envp)
	{
		msh->envp = set_pwd(msh->envp);
		msh->envp = set_shlvl(msh->envp);
	}
	else
		msh->envp = increment_shlvl(msh->envp);
	msh->prompt = MSH_PROMPT;
	msh->original_stdin = dup(STDIN_FILENO);
	msh->original_stdout = dup(STDOUT_FILENO);
	msh->original_stderr = dup(STDERR_FILENO);
}
