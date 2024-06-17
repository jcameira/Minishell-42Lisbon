/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/06/17 19:00:42 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->check all simple commands and redirections to see if there is anything
//in need of expansion, in particular for the presence of '$' to expand
//enviroment variables or '*' for wildcards
//->send the full command table with all expansions completed to the executor

#include <expander.h>

void	set_here_doc_expansion(t_redir_list **redir)
{
	int	i;

	i = -1;
	while ((*redir)->here_doc_limiter[++i])
	{
		if ((*redir)->here_doc_limiter[i] == '\''
			|| (*redir)->here_doc_limiter[i] == '"')
		{
			(*redir)->expand_here_doc = 1;
			break ;
		}
	}
}

int	need_expansion(char *content)
{
	int	i;

	i = -1;
	while (content[++i])
		if (content[i] == '\'' || content[i] == '"' || content[i] == '$'
			|| content[i] == '*')
			return (1);
	return (0);
}

char	*expand_content(t_minishell *msh, char **content)
{
	char	*expanded_content;
	int		inside_single_quotes;
	int		expanded_len;

	if (!need_expansion(*content))
		return (*content);
	expanded_len = expanded_str_len();
	return (expanded_content);
}

t_redir_list	*expand_redirs(t_minishell *msh, t_command_table *command_table)
{
	t_redir_list	*tmp_redir;

	tmp_redir = command_table->redirs;
	while (tmp_redir)
	{
		if (tmp_redir->type == HERE_DOC)
		{
			set_here_doc_expansion(&tmp_redir);
			tmp_redir->here_doc_limiter = expand_content(msh,
					tmp_redir->here_doc_limiter);
			if (!tmp_redir->here_doc_limiter)
				return (free_command_table(command_table), NULL);
		}
		else
		{
			tmp_redir->file = expand_content(msh, tmp_redir->file);
			if (!tmp_redir->file)
				return (free_command_table(command_table), NULL);
		}
		tmp_redir = tmp_redir->next;
	}
	return (command_table->redirs);
}

void	expander(t_minishell *msh, t_command_table *command_table)
{
	t_command_table	*tmp_table;
	int				i;

	tmp_table = command_table;
	while (tmp_table)
	{
		i = -1;
		while (tmp_table->simplecmd->arg_arr[++i])
		{
			tmp_table->simplecmd->arg_arr[i] = expand_content(msh,
					tmp_table->simplecmd->arg_arr[i]);
			if (!tmp_table->simplecmd->arg_arr[i])
				return (free_command_table(command_table));
		}
		tmp_table->redirs = expand_redirs(msh, tmp_table);
	}
	executor(msh, command_table);
}
