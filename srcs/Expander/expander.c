/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/19 16:56:23 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->check all simple commands and redirections to see if there is anything
//in need of expansion, in particular for the presence of '$' to expand
//enviroment variables or '*' for wildcards
//->send the full command table with all expansions completed to the executor

#include <expander.h>

int	set_ambiguous_redirect(char *content)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(content);
	if (!tmp)
		return (ft_putstr_fd(NO_SPACE, 2), -1);
	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == '"')
		{
			if (expansion_inside_quotes(tmp, i, '"', AMBIGUOUS))
				return (1);
			else
				skip_until_char(tmp, &i, '"');
		}
		else if (tmp[i] == '\'')
			skip_until_char(tmp, &i, '\'');
	}
	return (0);
}

char	*expand_content(t_minishell *msh, char *content)
{
	char	*expanded_content;
	int		expanded_len;

	expanded_content = ft_strdup(content);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free(content);
	expanded_len = quote_removal_str_len(expanded_content);
	expanded_content = remove_quotes_expansion(expanded_content, expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	expanded_len = parameter_expansion_str_len(msh, expanded_content);
	expanded_content = expand_parameter(msh, expanded_content, expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	expanded_len = wildcards_str_len(expanded_content);
	expanded_content = expand_wildcards(expanded_content, expanded_len,
			needs_wildcard_expansion(expanded_content));
	if (!expanded_content)
		return (free(expanded_content), NULL);
	return (expanded_content);
}

t_redir_list	*expand_redirs(t_minishell *msh, t_command_table *command_table)
{
	t_redir_list	*tmp_redir;

	tmp_redir = command_table->redirs;
	while (tmp_redir)
	{
		if (tmp_redir->type != HERE_DOC)
		{
			tmp_redir->ambiguous_redirect = set_ambiguous_redirect(tmp_redir->file);
			if (tmp_redir->ambiguous_redirect == -1)
				return (free_command_table(command_table), NULL);
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
	t_command_table			*tmp_table;
	t_final_command_table	*final_command_table;
	int						i;

	tmp_table = command_table;
	while (tmp_table)
	{
		if (tmp_table->simplecmd->arg_arr)
		{
			i = -1;
			while (tmp_table->simplecmd->arg_arr[++i])
			{
				tmp_table->simplecmd->arg_arr[i] = expand_content(msh,
						tmp_table->simplecmd->arg_arr[i]);
				if (!tmp_table->simplecmd->arg_arr[i])
					return (free_command_table(command_table));
			}
		}
		if (tmp_table->redirs->file || tmp_table->redirs->here_doc_limiter)
			tmp_table->redirs = expand_redirs(msh, tmp_table);
		tmp_table = tmp_table->next;
	}
	print_cmd_table(command_table);
	final_command_table = create_final_cmd_table(command_table);
	executor(msh, final_command_table);
}
