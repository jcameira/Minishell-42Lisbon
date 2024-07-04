/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_here_docs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 01:45:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 01:55:14 by jcameira         ###   ########.fr       */
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

void	handle_here_doc(t_minishell *msh, t_redir_list **redirs)
{
	char	*line;
	int		line_nbr;

	if (!(*redirs)->here_doc_limiter)
		return ;
	line_nbr = 0;
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
			break ;
		line = expansion_inside_here_doc(msh, line, (*redirs)->expand_here_doc);
		(*redirs)->here_doc_buffer = add_line_to_buffer(line,
				(*redirs)->here_doc_buffer);
		free(line);
		if (!(*redirs)->here_doc_buffer)
			return ;
	}
	free(line);
}
