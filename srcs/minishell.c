/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/30 15:13:34 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_sigint;

void	msh_loop(t_minishell *msh)
{
	char	*line;

	while (1)
	{
		interactive_signals_init();
		line = readline(msh->prompt);
		if (g_sigint)
		{
			g_sigint = !g_sigint;
			msh->exit_code = 130;
		}
		if (!line)
		{
			// ft_putstr_fd("exit\n", 1);
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
	g_sigint = 0;
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
