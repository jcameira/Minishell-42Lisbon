/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_here_docs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 01:45:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/01 21:32:00 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

extern int g_sigint;

char	*add_line_to_buffer(char *new_line, char *previous_buffer)
{
	char	*new_buffer;

	if (!previous_buffer)
		new_buffer = ft_strdup(new_line);
	else
		new_buffer = ft_strjoin(previous_buffer, new_line);
	free(previous_buffer);
	if (!new_buffer)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (new_buffer);
}

void	disable_quit_character(struct termios *old_term)
{
	struct	termios new_term;

	tcgetattr(STDIN_FILENO, old_term);
	new_term = *old_term;
	new_term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void	handle_here_doc(t_minishell *msh, t_command_table *table, t_redir_list **redirs, int *fd)
{
	char	*line;
	int		line_nbr;
	char	*line_nbr_str;

	line_nbr = 0;
	here_doc_signals_init();
	while (true && ++line_nbr)
	{
		ft_putstr_fd(HERE_DOC_INDICATOR, 2);
		line = get_next_line(INPUT);
		if (g_sigint)
			break ;
		if (!line)
		{
			line_nbr_str = ft_itoa(line_nbr);
			ft_putstr_fd(NO_HERE_DOC_LINE_START, 2);
			ft_putstr_fd(line_nbr_str, 2);
			ft_putstr_fd(NO_HERE_DOC_LINE_MIDDLE, 2);
			ft_putstr_fd((*redirs)->here_doc_limiter, 2);
			ft_putstr_fd(NO_HERE_DOC_LINE_END, 2);
			free(line_nbr_str);
			break ;
		}
		if (line[0] != '\n' && !ft_strncmp(line, (*redirs)->here_doc_limiter, ft_strlen(line) - 1))
		{
			free(line);
			break ;
		}
		line = expansion_inside_here_doc(msh, table, line, (*redirs)->expand_here_doc);
		ft_putstr_fd(line, fd[WRITE]);
		free(line);
	}
	close(fd[READ]);
	close(fd[WRITE]);
}

int	fork_here_doc(t_minishell *msh, t_ast *root,
	t_command_table *command_table, t_redir_list **redirs)
{
	pid_t	fork_here_doc;
	int		status;
	int		fd[2];

	if (pipe(fd) < 0)
		return (ft_putstr_fd(OPEN_PIPE_ERROR, 2), -1);
	fork_here_doc = fork();
	if (fork_here_doc < 0)
		return (ft_putstr_fd(FORK_ERROR, 2), -1);
	if (fork_here_doc == 0)
	{
		handle_here_doc(msh, command_table, redirs, fd);
		free_command_table(command_table, 1);
		if (*redirs)
		{
			free((*redirs)->here_doc_limiter);
			free(*redirs);
		}
		free_ast(root->original_root);
		if (g_sigint)
			exit_shell(msh, 130);
		exit_shell(msh, EXIT_SUCCESS);
	}
	close(fd[WRITE]);
	waitpid(fork_here_doc, &status, 0);
	if (WEXITSTATUS(status) == 130)
		g_sigint = !g_sigint;
	return (fd[READ]);
}
