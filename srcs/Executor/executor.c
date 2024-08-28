/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/08/26 23:14:52 by marvin           ###   ########.fr       */
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

int	executor(t_minishell *msh, t_final_command_table *final_command_table)
{
	int	pipeline_start;
	int	pipeline_size;
	pid_t *pid;
	int	in_pipe[2];
	int	out_pipe[2];
	int i;
	
	if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1)
	{
		free_f_command_table(final_command_table);
		return (ft_putstr(OPEN_PIPE_ERROR, 2));
	}
	pipeline_start = 1;
	while (final_command_table->simplecmd)
	{
		if (pipeline_start)
		{
			pipeline_size = get_pipeline_size(final_command_table);
			pipeline_start = !pipeline_start;
			pid = malloc(sizeof (pid_t) * pipeline_size);
			if (!pid)
				return(EXIT_FAILURE);
			i = 0;
		}
		if (!set_in(final_command_table) || !set_out(final_command_table))
			break ;
		if (final_command_table->infile_fd == -2 || (final_command_table->previous_symbol != PIPE))
			final_command_table->infile_fd = in_pipe[READ];
		if (final_command_table->outfile_fd == -2 || (final_command_table->next_symbol != PIPE))
			final_command_table->outfile_fd = out_pipe[WRITE];
		executor_simplecommand(msh, final_command_table, pid, &i);
		close(in_pipe[READ]);
		close(in_pipe[WRITE]);
		in_pipe = out_pipe;
		if (pipe(out_pipe) == -1)
		{
			free_f_command_table(final_command_table);
			return (ft_putstr(OPEN_PIPE_ERROR, 2));
		}
		i++;
		final_command_table = final_command_table->next;
	}
	
	free_f_command_table(final_command_table);
	return ();
}

int get_pipeline_size(t_final_command_table *final_command_table);
{
	int	size;
	t_final_command_table *tmp;

	size = 0;
	tmp = final_command_table;
	while (tmp->next_symbol == PIPE)
	{
		tmp = tmp->next_symbol;
		size++;
	}
	return (size);
}

int	set_in(t_final_command_table *final_command_table)
{
	if (final_command_table->in_type == NO_TYPE)
		final_command_table->infile_fd = -2;
	else if (final_command_table->in_type == INFILE)
		final_command_table->infile_fd = open(final_command_table->infile,
			O_RDONLY);
	if (final_command_table->infile == -1)
		return (ft_putstr(OPEN_IN_ERROR, 2), 0);
	return (1);
}

int	set_out(t_final_command_table *final_command_table)
{
	if (final_command_table->out_type == NO_TYPE)
		final_command_table->outfile_fd = -2;
	else if (final_command_table->out_type == OUTFILE)
		final_command_table->outfile_fd = open(final_command_table->outfile,
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (final_command_table->out_type == APPEND)
		final_command_table->outfile_fd = open(final_command_table->outfile,
			O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (final_command_table->outfile == -1)
		return (ft_putstr(OPEN_OUT_ERROR, 2), 0);
	return (1);
}

void	executor_simplecommand(t_minishell *msh, t_final_command_table *final_command_table, pid_t *pid, int *i)
{
			//if((final_command_table->next_symbol != AND && excev() != 1) || (final_command_table->next_symbol != OR && excev() == 1))
		//	//sair

		(pid)[i] = fork();
		if ((pid)[i] == 0)
			child(msh, final_command_table);

//	//	//executar singlecomand pipex do joao bonus
//	//	if(final_command_table->next_symbol != PIPE)
//	//		break;
//	//}
//		
}
void child(t_minishell *msh, t_final_command_table *final_command_table)
{
	dup2(final_command_table->infile_fd, 0);
	dup2(final_command_table->outfile_fd, 1);

	if (!strncmp(cmd_args[0], "./", 2))
		path = cmd_args[0];
	else if (!(*envp))
		path = ft_strjoin(DEFAULT_CMD_PATH, cmd_args[0]);
	else
		path = find_cmd_path(envp, cmd_args[0]);
	execute_cmd_bonus(info, path, cmd_args, envp);
}