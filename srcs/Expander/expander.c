/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 15:28:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

char	*word_split(char *str, char ***content, int i)
{
	char	**new_content;
	char	**tmp;
	int		arr_size;
	int		new_size;
	int		j;
	int		k;

	if (!space_not_in_quotes(str))
		return (str);
	tmp = ft_split(str, ' ');
	if (!tmp)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	arr_size = array_size(tmp);
	if (arr_size == 1)
		return (str);
	new_size = array_size(*content) + arr_size;
	new_content = malloc(sizeof(char *) * new_size);
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	j = -1;
	k = -1;
	while ((*content)[++j])
	{
		if (j == i)
		{
			i = -1;
			while (tmp[++i])
			{
				new_content[++k] = ft_strdup(tmp[i]);
				if (!new_content[k])
					return (free_arr(new_content), free_arr(tmp),
						ft_putstr_fd(NO_SPACE, 2), NULL);
			}
		}
		else
		{
			new_content[++k] = ft_strdup((*content)[j]);
			if (!new_content[k])
				return (free_arr(new_content), free_arr(tmp),
					ft_putstr_fd(NO_SPACE, 2), NULL);
		}
	}
	free(str);
	str = ft_strdup(tmp[0]);
	if (!str)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free_arr(tmp);
	new_content[++k] = NULL;
	free_arr(*content);
	*content = new_content;
	return (str);
}

char	*expand_content(t_minishell *msh, t_command_table *table,
	char ***content, int i)
{
	char	*expanded_content;
	int		expanded_len;

	expanded_content = ft_strdup((*content)[i]);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	expanded_len = parameter_expansion_str_len(msh, table, expanded_content);
	expanded_content = expand_parameter(msh, table, expanded_content,
			expanded_len);
	if (!expanded_content)
		return (free((*content)[i]), NULL);
	expanded_content = word_split(expanded_content, content, i);
	if (!expanded_content)
		return (free((*content)[i]), NULL);
	expanded_len = wildcards_str_len(expanded_content);
	expanded_content = expand_wildcards(expanded_content, expanded_len,
			needs_wildcard_expansion(expanded_content));
	if (!expanded_content)
		return (free((*content)[i]), NULL);
	expanded_len = quote_removal_str_len(expanded_content);
	expanded_content = remove_quotes_expansion(expanded_content, expanded_len);
	if (!expanded_content)
		return (free((*content)[i]), NULL);
	free((*content)[i]);
	return (expanded_content);
}

t_redir_list	*expand_redirs(t_minishell *msh, t_command_table *command_table)
{
	t_redir_list	*tmp_redir;
	char			**tmp_file;

	tmp_redir = command_table->redirs;
	while (tmp_redir)
	{
		if (tmp_redir->type != HERE_DOC)
		{
			tmp_redir->ambiguous_redirect = set_ambiguous_redirect(
					msh, command_table, tmp_redir->file);
			if (tmp_redir->ambiguous_redirect == -1)
				return (free_command_table(command_table, 1), NULL);
			tmp_file = &tmp_redir->file;
			if (tmp_redir->ambiguous_redirect)
				tmp_redir->file = remove_quotes_expansion(tmp_redir->file,
						quote_removal_str_len(tmp_redir->file));
			else
				tmp_redir->file = expand_content(msh, command_table,
						&tmp_file, 0);
			if (!tmp_redir->file)
				return (free_command_table(command_table, 1), NULL);
		}
		tmp_redir = tmp_redir->next;
	}
	return (command_table->redirs);
}

int	expander(t_minishell *msh, t_command_table *command_table)
{
	t_command_table			*tmp_table;
	t_final_cmd_table		*final_cmd_table;
	char					*tmp; 
	int						i;

	tmp_table = command_table;
	while (tmp_table)
	{
		if (tmp_table->simplecmd)
		{
			i = -1;
			while (tmp_table->simplecmd->arg_arr[++i])
			{
				tmp = expand_content(msh,
						tmp_table, &tmp_table->simplecmd->arg_arr, i);
				tmp_table->simplecmd->arg_arr[i] = ft_strdup(tmp);
				free(tmp);
				if (!tmp_table->simplecmd->arg_arr[i])
					return (free_command_table(command_table, 1), -1);
			}
		}
		if (tmp_table->redirs && (tmp_table->redirs->file
				|| tmp_table->redirs->here_doc_limiter))
			tmp_table->redirs = expand_redirs(msh, tmp_table);
		tmp_table = tmp_table->next;
	}
	final_cmd_table = create_final_cmd_table(command_table);
	return (executor(msh, final_cmd_table));
}
