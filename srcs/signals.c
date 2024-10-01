/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 15:51:47 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/01 21:29:11 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

extern int g_sigint;

void	interactive_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint = !g_sigint;
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
		ft_putchar_fd('\n', 1);
	}
}