/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/03 20:59:40 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_signal;

void	msh_loop(t_minishell *msh)
{
	char	*line;

	while (1)
	{
		interactive_signals_init();
		line = readline(msh->prompt);
		if (g_signal)
		{
			msh->exit_code = g_signal;
			g_signal = 0;
		}
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			exit_shell(msh, EXIT_SUCCESS);
		}
		if (!line[0] || full_white_space(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		msh->exit_code = lexer(msh, line);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*msh;

	(void) argv;
	g_signal = 0;
	if (argc != 1)
		return (ft_putstr_fd(WRONG_ARG_N, 2), 1);
	msh = malloc(sizeof(t_minishell));
	if (!msh)
		return (ft_putstr_fd(NO_SPACE, 2), 1);
	minishell_init(msh, envp);
	if (!msh->envp)
		return (1);
	msh_loop(msh);
	return (0);
}
