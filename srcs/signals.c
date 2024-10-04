/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 15:51:47 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/04 21:47:38 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

extern int g_signal;

void	interactive_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	child_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		// ft_putchar_fd('\n', 1);
		write(1, "\n", 1);
	}
	if (sig == SIGQUIT)
	{
		g_signal = 131;
	}
}