/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:12:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/12 16:19:53 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signals_init(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = interactive_handler;
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaction(SIGINT, &sa_int, 0);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &sa_quit, 0);
}

void	minishell_init(t_minishell *msh, char **envp)
{
	msh->envp = arrdup(envp);
	if (!msh->envp)
		return ;
	if (msh->envp && !msh->envp[0])
	{
		msh->envp = set_pwd(msh->envp);
		if (!msh->envp)
			return (free(msh->envp));
		msh->envp = set_shlvl(msh->envp);
		if (!msh->envp)
			return (free(msh->envp));
	}
	else
	{
		msh->envp = increment_shlvl(msh->envp);
		if (!msh->envp)
			return (free(msh->envp));
	}
	msh->prompt = MSH_PROMPT;
	msh->original_stdin = dup(STDIN_FILENO);
	msh->original_stdout = dup(STDOUT_FILENO);
	msh->original_stderr = dup(STDERR_FILENO);
}
