/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/13 17:17:30 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_signal;

void	msh_loop(t_minishell *msh)
{
	char	*line;

	interactive_signals_init();
	while (1)
	{
		g_signal = 0;
		line = readline(msh->prompt);
		if (!line)
		{
			printf("exit\n");
			exit_shell(msh, SUCCESS);
		}
		if (!line[0] || full_white_space(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		lexer(msh, line);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*msh;

	(void) argv;
	if (argc != 1)
		return (ft_putstr_fd(WRONG_ARG_N, 2), 1);
	msh = malloc(sizeof(t_minishell));
	if (!msh)
		return (ft_putstr_fd(NO_SPACE, 2),1);
	minishell_init(msh, envp);
	if (!msh->envp)
		return (1);
	msh_loop(msh);
	free_arr(msh->envp);
	return (0);
}
