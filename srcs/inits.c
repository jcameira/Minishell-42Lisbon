/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:12:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/01 21:53:13 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signals_init(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = interactive_handler;
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, 0);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &sa, 0);
}

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
