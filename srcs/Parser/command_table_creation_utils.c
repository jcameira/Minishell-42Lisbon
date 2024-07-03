/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_creation_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:24:54 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/03 22:18:38 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_table_node_type	set_table_node_type(t_ast *root)
{
	if (!ft_strncmp(root->content, "&&", 2))
		return (TABLE_AND);
	else if (!ft_strncmp(root->content, "||", 2))
		return (TABLE_OR);
	else if (root->type == PIPE_SEQUENCE)
		return (TABLE_PIPE);
	else if (root->type == SUBSHELL)
		return (TABLE_SUBSHELL);
	else
		return (TABLE_NO_TYPE);
}

t_redir_type	set_redir_type(char *content)
{
	if (!ft_strncmp(content, ">>", 2))
		return (APPEND);
	else if (!ft_strncmp(content, "<<", 2))
		return (HERE_DOC);
	else if (!ft_strncmp(content, ">", 2))
		return (OUTFILE);
	else
		return (INFILE);
}

char	*set_redir_str(char *dest, t_ast **root)
{
	if ((*root)->right->type == REDIRECTION)
	{
		dest = ft_strdup((*root)->right->left->content);
		(*root)->right->left->visited = 1;
	}
	else
	{
		dest = ft_strdup((*root)->right->content);
		(*root)->right->visited = 1;
	}
	if (!dest)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (dest);
}

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

int	parameter_expansion_here_doc_str_len(t_minishell *msh, char *content)
{
	int	real_len;
	int	i;

	real_len = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '$' && content[i + 1] && isenvchar(content[i + 1]))
			real_len += get_env_variable_len(msh, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

char	*expand_parameter_inside_here_doc(t_minishell *msh, char *content, int len)
{
	char	*new_content;
	char	*env_name;
	char	*env_value;
	int		i;
	int		j;

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '$' && content[i + 1])
		{
			env_name = get_env_name(content, &i);
			if (!env_name)
			{
				free(content);
				return (free(new_content), NULL);
			}
			env_value = get_env_value(msh, env_name);
			if (!env_value)
			{
				free(env_name);
				free(content);
				return (free(new_content), NULL);
			}
			new_content = add_expanded_var(env_value, new_content, &j);
			free(env_name);
			free(env_value);
		}
		else
			new_content[++j] = content[i];
	}
	new_content[++j] = '\0';
	free(content);
	return (new_content);
}

char	*expansion_inside_here_doc(t_minishell *msh, char *content, int flag)
{
	char	*expanded_content;
	int		expanded_len;

	if (flag)
		return (content);
	expanded_content = ft_strdup(content);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free(content);
	expanded_len = parameter_expansion_here_doc_str_len(msh, expanded_content);
	expanded_content = expand_parameter_inside_here_doc(msh, expanded_content, expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	return (expanded_content);
}

void	handle_here_doc(t_minishell *msh, t_redir_list **redirs)
{
	char	*line;
	int		line_nbr;

	if (!(*redirs)->here_doc_limiter)
		return ;
	line_nbr = 1;
	while (true)
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
		(*redirs)->here_doc_buffer = add_line_to_buffer(line, (*redirs)->here_doc_buffer);
		free(line);
		if (!(*redirs)->here_doc_buffer)
			return ;
		line_nbr++;
	}
	free(line);
}

t_redir_list	*set_redir_values(t_minishell *msh, t_ast **root, t_redir_list *redirs)
{
	redirs->type = set_redir_type((*root)->content);
	if (redirs->type == HERE_DOC)
	{
		redirs->here_doc_limiter = set_redir_str(redirs->here_doc_limiter,
				root);
		set_here_doc_expansion(&redirs);
		redirs->here_doc_limiter = remove_quotes(redirs->here_doc_limiter);
		handle_here_doc(msh, &redirs);
	}
	else
		redirs->file = set_redir_str(redirs->file, root);
	if (!redirs->here_doc_limiter && !redirs->file)
		return (NULL);
	return (redirs);
}
