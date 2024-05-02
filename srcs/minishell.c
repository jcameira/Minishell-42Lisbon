/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/02 03:36:54 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->preferably duplicate stdin and stdout now to be able to reset later
//	(still debatable, could easily be done only on the execution stage) - KINDA DONE
//->copy enviroment variables into minishell struct - DONE
//->increment shell level (if no enviroment variables then set to 1) - DONE
//->if no enviroment variables set PWD, else update PWD - DONE
//->initialize any other needed variables (about the single global
//variables we can use, I was thinking of using it only to save the
//latest exit code, but could probably scale it to a struct to handle any
//signal related stuff) - MOSTLY DONE, TRYING TO FIGURE OUT IF SOMETHING IS LEFT
//->initialize signals
//	->Interractive mode (Minishell prompt)
//		->Ctrl-C should write "^C" and show new prompt, if anything is
//		  written in the previous prompt write "^C" and show new prompt
//		  as well but don't add to history
//		->Ctrl-D should exit minishell on empty prompt or indicates EOF
//		  if something is already written on the prompt, which effectively
//		  does nothing in this case
//		->Ctrl-\ does nothing with or without anything written in the
//		  prompt
//	->During execution of other processes
//		->Ctrl-C should send a SIGINT signal, effectively ending the
//		  current process in a controlled manner, meaning it gives the
//		  opportunity for cleanup, it should also write "^C"
//		->Ctrl-D should indicate EOF which effectively ends user input
//		  dependent processes like cat (if we are in here documents this
//		  should give a warning since it ended without reaching the actual
//		  delimiter given in the command line)
//		->Ctrl-\ should send a SIGQUIT signal, which effectively does the
//		  same as SIGINT but produces a core dump on top of it, it should
//		  write "^\Quit (core dumped)" as well
//->start minishell loop (show Minishell prompt and wait for user input,
//send the line written to the lexer) - IN PROCESS

#include <minishell.h>
#include <lexer.h>

void	msh_loop(t_minishell *msh)
{
	char	*line;

	signals_init();
	while (1)
	{
		line = readline(msh->prompt);
		if (!line)
			exit_shell(msh);
		if (!line[0] || is_white_space(line))
		{
			free(line);
			printf("Here\n");
			continue ;
		}
		add_history(line);
		lexer(line);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	msh;

	(void) argv;
	if (argc != 1)
		return (ft_putstr_fd(WRONG_ARG_N, 2), 1);
	minishell_init(&msh, envp);
	msh_loop(&msh);
	free_arr(msh.envp);
	return (0);
}
