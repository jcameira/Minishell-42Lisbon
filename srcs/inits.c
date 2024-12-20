/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:12:48 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 17:12:32 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	interactive_signals_init(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = interactive_handler;
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaction(SIGINT, &sa_int, 0);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &sa_quit, 0);
}

void	here_doc_signals_init(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = here_handler;
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaction(SIGINT, &sa_int, 0);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &sa_quit, 0);
}

void	child_signals_init(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = child_handler;
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaction(SIGINT, &sa_int, 0);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = child_handler;
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &sa_quit, 0);
}

t_minishell	*init_envs(t_minishell *msh, char **envp)
{
	msh->envp = arrdup(envp);
	msh->export_list = arrdup(envp);
	if (!msh->envp || !msh->export_list)
		return (free_arr(msh->envp), free_arr(msh->export_list), NULL);
	bubble_sort(msh->export_list);
	msh->private_path = NULL;
	if (msh->envp && !msh->envp[0])
	{
		msh->private_path = strdup(PRIVATE_PATH);
		if (!msh->private_path)
			return (free_arr(msh->envp), free_arr(msh->export_list), NULL);
	}
	return (msh);
}

void	minishell_init(t_minishell *msh, char **envp)
{
	msh = init_envs(msh, envp);
	if (!msh)
		return ;
	if (msh->envp && !msh->envp[0])
	{
		msh->envp = set_pwd(msh->envp);
		if (!msh->envp)
			return (free(msh->envp), free(msh->export_list));
		msh->envp = set_shlvl(msh->envp);
		msh->export_list = set_shlvl(msh->export_list);
		if (!msh->envp)
			return (free(msh->envp), free(msh->export_list));
	}
	else
	{
		msh->envp = increment_shlvl(msh->envp);
		msh->export_list = increment_shlvl(msh->export_list);
		if (!msh->envp || !msh->export_list)
			return (free_arr(msh->envp), free_arr(msh->export_list));
	}
	msh->prompt = MSH_PROMPT;
	msh->exit_code = 0;
	msh->original_stdin = dup(STDIN_FILENO);
	msh->original_stdout = dup(STDOUT_FILENO);
	msh->original_stderr = dup(STDERR_FILENO);
}
