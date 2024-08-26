/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/08/26 19:53:43 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->just execute the damn thing, need to think of a much more computation light
//way to do it then what was done in pipex, we can't just open a million pipes
//all at the same time for example



//1. Recebe tabela final (que é uma lista). A tabela já tem os comandos pela ordem que devem ser executados (atenção &&, || e |)
//2. Executar pipeline
//2.1 Loop through commands
//2.2 Check if next_symbol is pipe, if yes then pipe(), (only have 2 open pipes max)
//2.3 Fork() to execute command
//2.4 Check if there are in and/or out redirections
//2.5 Open necessary files, check if they were successful
//2.6 dup2() with those fd's (if no in or out fd's use appropriate pipe fd's (if first command read from input, if last command write to output))
//2.7 se é built in ou não. Se se sim chamo buitin_arr se não execve.
//2.8 If execve, first check if the command word is  viable command
//2.9 Save last commands exit code

//3. Executar && ou ||
//3.1 if next_symbol is &&
//3.1.1 if last command from last pipeline failed, stop here
//3.1.2 if last command from last pipeline success, execute go back to point 2.
//3.2 if next_symbol is ||
//3.2.1 if last command from last pipeline success, stop here
//3.2.2 if last command from last pipeline failed, execute go back to point 2.

//4. clean up everything necessary (no need to cleanup anything that succeeds in execve, but everything else yes)

#include <executor.h>

void	executor(t_minishell *msh, t_final_command_table *final_command_table)
{
	(void)msh;
	while (final_command_table->simplecmd)
	{
		if (!set_in(final_command_table) || !set_out(final_command_table))
		{
			final_command_table = final_command_table->next;
			continue ;
		}
		//executor_simplecommand(msh, final_command_table);
		//if((final_command_table->next_symbol != AND && excev() != 1) || (final_command_table->next_symbol != OR && excev() == 1))
		//	//sair
		//final_command_table = final_command_table->next;
	}
	free_f_command_table(final_command_table);
}

int	set_in(t_final_command_table *final_command_table)
{
	t_final_command_table	*tmp;

	tmp = final_command_table;
	while (final_command_table)
	{
		if (final_command_table->here_doc_fd > -1)
			here_doc_init(argv[2], info);
		else
			info->infile_fd = open(argv[1], O_RDONLY);
	}
}

int	set_out(t_final_command_table *final_command_table)

//void	executor_simplecommand(t_minishell *msh,
//	t_final_command_table *final_command_table)
//{
//	int	in_pipe[2];
//	int	out_pipe[2];
//
//	
//	//while (final_command_table->simplecmd)
//	//{
//	//	//executar singlecomand pipex do joao bonus
//	//	if(final_command_table->next_symbol != PIPE)
//	//		break;
//	//}
//		
//}