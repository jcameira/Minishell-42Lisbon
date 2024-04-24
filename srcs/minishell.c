/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/24 21:03:06 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 1)
		return (error_message(WRONG_ARG_N), 1);
	(void) argv;
	//->preferably duplicate stdin and stdout now to be able to reset later
	//(still debatable, could easily be done only on the execution stage)
	//->copy enviroment variables into minishell struct
	//->increment shell level (if no enviroment variables then set to 1)
	//->if no enviroment variables set PWD, else update PWD
	//->initialize any other needed variables (about the single global
	//variables we can use, I was thinking of using it only to save the
	//latest exit code, but could probably scale it to a struct to handle any
	//signal related stuff)
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
	//send the line written to the lexer)
	return (0);
}