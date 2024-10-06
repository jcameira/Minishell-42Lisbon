/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_here_docs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 01:45:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:12 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	here_doc_warning(t_redir_list **redirs, int line_nbr)
{
	char	*line_nbr_str;

	line_nbr_str = ft_itoa(line_nbr);
	ft_putstr_fd(NO_HERE_DOC_LINE_START, 2);
	ft_putstr_fd(line_nbr_str, 2);
	ft_putstr_fd(NO_HERE_DOC_LINE_MIDDLE, 2);
	ft_putstr_fd((*redirs)->here_doc_limiter, 2);
	ft_putstr_fd(NO_HERE_DOC_LINE_END, 2);
	free(line_nbr_str);
}

void	handle_here_doc(t_minishell *msh, t_command_table *table,
	t_redir_list **redirs, int *fd)
{
	char	*line;
	int		line_nbr;

	line_nbr = 0;
	while (true && ++line_nbr)
	{
		line = readline("> ");
		if (g_signal)
			break ;
		if (!line)
		{
			here_doc_warning(redirs, line_nbr);
			break ;
		}
		if (!ft_strncmp(line, (*redirs)->here_doc_limiter, ft_strlen(line)))
		{
			free(line);
			break ;
		}
		line = expansion_inside_here_doc(msh, table, line,
				(*redirs)->expand_here_doc);
		ft_putendl_fd(line, fd[WRITE]);
		free(line);
	}
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
		here_doc_signals_init();
		close(fd[READ]);
		handle_here_doc(msh, command_table, redirs, fd);
		here_doc_fork_exit(msh, root, command_table, redirs);
	}
	close(fd[WRITE]);
	waitpid(fork_here_doc, &status, 0);
	if (WEXITSTATUS(status) == 130)
		g_signal = 130;
	return (fd[READ]);
}
