/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 15:51:47 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 20:31:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

extern int g_sigint;

void	interactive_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint = !g_sigint;
		//write(1, "\n", 1);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	here_doc_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint = !g_sigint;
		//write(1, "\n", 1);
		ft_putchar_fd('\n', 1);
		//rl_on_new_line();
		//rl_replace_line("", 0);
		//rl_redisplay();
	}
}