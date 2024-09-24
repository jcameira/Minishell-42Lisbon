/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_here_docs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 01:45:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 02:06:57 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

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

void	handle_here_doc(t_minishell *msh, t_command_table *table, t_redir_list **redirs, int *fd)
{
	char	*line;
	int		line_nbr;

	line_nbr = 0;
	child_signals_init();
	while (true && ++line_nbr)
	{
		ft_putstr_fd(HERE_DOC_INDICATOR, 1);
		line = get_next_line(INPUT);
		if (!line)
		{
			printf(NO_HERE_DOC_LINE, line_nbr, (*redirs)->here_doc_limiter);
			break ;
		}
		if (!ft_strncmp(line, (*redirs)->here_doc_limiter, ft_strlen(line) - 1))
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
		exit_shell(msh, FAILURE);
	}
	close(fd[WRITE]);
	waitpid(fork_here_doc, NULL, 0);
	return (fd[READ]);
}
