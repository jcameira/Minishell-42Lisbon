/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/23 20:55:08 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	msh_loop(t_minishell *msh)
{
	char	*line;

	while (1)
	{
		interactive_signals_init();
		line = readline(msh->prompt);
		if (!line)
		{
			printf("exit\n");
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
