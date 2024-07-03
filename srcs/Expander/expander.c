/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/03 21:56:33 by jcameira         ###   ########.fr       */
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

int	isenvchar(int c)
{
	return (ft_isalnum(c) || c == 95);
}

char *get_env_value(t_minishell *msh, char *env_name)
{
	char	**tmp_envp;
	char	*env_value;
	int		env_name_len;
	int		i;

	tmp_envp = msh->envp;
	env_name_len = ft_strlen(env_name);
	i = -1;
	while (tmp_envp[++i])
		if (!ft_strncmp(tmp_envp[i], env_name, env_name_len)
			&& tmp_envp[i][env_name_len] == '=')
			break ;
	if (!tmp_envp[i])
	{
		env_value = malloc(sizeof(char));
		if (!env_value)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		env_value[0] = '\0';
		return (env_value);
	}
	env_value = ft_substr(tmp_envp[i], env_name_len + 1,
		ft_strlen(&tmp_envp[i][env_name_len]));
	if (!env_value)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (env_value);
}

char	*get_env_name(char *content, int *i)
{
	int		env_name_start;
	char	*env_name;

	env_name_start = ++(*i);
	while (content[*i] && isenvchar(content[*i]))
		(*i)++;
	env_name = ft_substr(content, env_name_start, (*i) - env_name_start);
	if (!env_name)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	(*i)--;
	return(env_name);
}

int	get_env_variable_len(t_minishell *msh, char *content, int *i)
{
	int		env_len;
	char	*env_name;
	char	*env_value;
	

	env_name = get_env_name(content, i);
	if (!env_name)
		return (-1);
	env_value = get_env_value(msh, env_name);
	free(env_name);
	if (!env_value)
		return (-1);
	env_len = ft_strlen(env_value);
	free(env_value);
	return (env_len);
}

char	*add_expanded_var(char *env_value, char *expanded_content, int *j)
{
	int		i;

	i = -1;
	while (env_value[++i])
		expanded_content[++(*j)] = env_value[i];
	return (expanded_content);
}

int	needs_quote_removal(char *content)
{
	int	i;

	i = -1;
	while (content[++i])
		if (content[i] == '\'' || content[i] == '"')
			return (1);
	return (0);
}

int	needs_parameter_expansion(char *content)
{
	int	inside_s_quotes;
	int	inside_d_quotes;
	int	i;

	inside_d_quotes = 0;
	inside_s_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '"' && !inside_s_quotes)
			inside_s_quotes = !inside_d_quotes;
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		if (content[i] == '$' && !inside_s_quotes)
			return (1);
	}
	return (0);
}

int	expansion_inside_quotes(char *content, int i, char c, t_quote_flag flag)
{
	if (flag == QUOTES)
	{
		while (content[++i])
		{
			if (content[i] == c)
				break ;
			if (c == '\'' && (content[i] == '$' || content[i] == '*'))
				return (1);
			else if (c == '"' && content[i] == '*')
				return (1);
		}
	}
	else if (flag == PARAMETERS)
	{
		while (content[++i])
			if ((c == '"' || c == '\'') && content[i] == '*')
				return (1);
	}
	return (0);
}

int	len_inside_quotes(char *content, int *i, char c)
{
	int	len;

	len = 2;
	while (content[++(*i)] != c)
		len++;
	return (len);
}

int	quote_removal_str_len(char *content)
{
	int	real_len;
	int	inside_s_quotes;
	int	inside_d_quotes;
	int	i;

	real_len = 0;
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
		{
			inside_s_quotes = !inside_s_quotes;
			if(inside_s_quotes && expansion_inside_quotes(content, i, '\'', QUOTES))
				real_len += len_inside_quotes(content, &i, '\'');
		}
		else if (content[i] == '"' && !inside_s_quotes)
		{
			inside_d_quotes = !inside_d_quotes;
			if(inside_d_quotes && expansion_inside_quotes(content, i, '"', QUOTES))
				real_len += len_inside_quotes(content, &i, '"');
		}
		else
			real_len++;
	}
	return (real_len);
}

void	add_content_inside_quotes(char *content, char *new_content, int *i, int *j)
{
	char quote_type;

	quote_type = content[*i];
	new_content[++(*j)] = content[*i];
	while (content[++(*i)] != quote_type)
		new_content[++(*j)] = content[*i];
	new_content[++(*j)] = content[*i];
}

char	*remove_quotes_expansion(char *content, int len)
{
	char	*new_content;
	int		inside_s_quotes;
	int		inside_d_quotes;
	int		j;
	int		i;

	new_content = malloc(sizeof(char) * (len + 1));
	if(!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
		{
			inside_s_quotes = !inside_s_quotes;
			if (inside_s_quotes && expansion_inside_quotes(content, i, '\'', QUOTES))
				add_content_inside_quotes(content, new_content, &i, &j);
		}
		else if (content[i] == '"' && !inside_s_quotes)
		{
			inside_d_quotes = !inside_d_quotes;
			if (inside_d_quotes && expansion_inside_quotes(content, i, '"', QUOTES))
				add_content_inside_quotes(content, new_content, &i, &j);
		}
		else
			new_content[++j] = content[i];
	}
	new_content[++j] = '\0';
	free(content);
	return (new_content);
}

int	parameter_expansion_str_len(t_minishell *msh, char *content)
{
	int	real_len;
	int	inside_s_quotes;
	int	inside_d_quotes;
	int	had_expansion_in_quotes;
	int	i;

	real_len = 0;
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	had_expansion_in_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
		{
			inside_s_quotes = !inside_s_quotes;
			if(inside_s_quotes && expansion_inside_quotes(content, i, '\'', PARAMETERS))
				real_len += len_inside_quotes(content, &i, '\'');
		}
		else if (content[i] == '"' && !inside_s_quotes)
		{
			inside_d_quotes = !inside_d_quotes;
			if(inside_d_quotes && expansion_inside_quotes(content, i, '"', PARAMETERS))
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				real_len++;
			}
			if (!inside_d_quotes && had_expansion_in_quotes)
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				real_len++;
			}
		}
		else if (content[i] == '$' && content[i + 1]
			&& isenvchar(content[i + 1]) && !inside_s_quotes)
			real_len += get_env_variable_len(msh, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

char	*expand_parameter(t_minishell *msh, char *content, int len)
{
	char	*new_content;
	char	*env_name;
	char	*env_value;
	int		inside_s_quotes;
	int		inside_d_quotes;
	int		had_expansion_in_quotes;
	int		i;
	int		j;

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	had_expansion_in_quotes = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
		{
			inside_s_quotes = !inside_s_quotes;
			if(inside_s_quotes && expansion_inside_quotes(content, i, '\'', PARAMETERS))
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				new_content[++j] = content[i];
			}
			if (!inside_s_quotes && had_expansion_in_quotes)
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				new_content[++j] = content[i];
			}
		}
		else if (content[i] == '"' && !inside_s_quotes)
		{
			inside_d_quotes = !inside_d_quotes;
			if(inside_d_quotes && expansion_inside_quotes(content, i, '"', PARAMETERS))
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				new_content[++j] = content[i];
			}
			if (!inside_d_quotes && had_expansion_in_quotes)
			{
				had_expansion_in_quotes = !had_expansion_in_quotes;
				new_content[++j] = content[i];
			}
		}
		else if (content[i] == '$' && content[i + 1] && !inside_s_quotes)
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
			if (!inside_d_quotes || (inside_d_quotes && env_name[0]))
				new_content = add_expanded_var(env_value, new_content, &j);
			else
				new_content[++j] = content[i];
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

int match_wildcard_pattern(char *pattern, char *file)
{
    if (!(*pattern) && !(*file))
        return (1);
    if (*pattern == '*')
        return (match_wildcard_pattern(pattern + 1, file) || (*file && match_wildcard_pattern(pattern, file + 1)));
    if (*pattern == *file)
        return (match_wildcard_pattern(pattern + 1, file + 1));
    return (0);
}

int	get_wildcards_len(char *content)
{
	int 			real_len;
	char			*cwd;
	DIR				*directory;
	struct dirent	*file;

	real_len = 0;
	cwd = getcwd(NULL, 0);
	directory = opendir(cwd);
	file = readdir(directory);
	while (file)
	{
		printf("File name -> %s\n", file->d_name);
		if (!ft_strcmp(content, "*") && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			printf("File name match 2 -> %s\n", file->d_name);
			if (real_len > 0)
				real_len++;
			real_len += ft_strlen(file->d_name);
		}
		file = readdir(directory);
	}
	printf("Lenght of all file names -> %d\n", real_len);
	free(cwd);
	closedir(directory);
	return (real_len);
}

int	wildcards_str_len(char *content)
{
	int	real_len;
	int	inside_s_quotes;
	int	inside_d_quotes;
	int	i;

	real_len = 0;
	inside_d_quotes = 0;
	inside_s_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		else if (content[i] == '"' && !inside_s_quotes)
			inside_d_quotes = !inside_d_quotes;
		else if (content[i] == '*' && !inside_s_quotes && !inside_d_quotes)
		{
			real_len += get_wildcards_len(content);
			return (real_len);
		}
		else
			real_len++;
	}
	return (real_len);
}

char	*add_wildcard_content(char *content)
{
	char			*new_content;
	char			*tmp;
	char			*cwd;
	DIR				*directory;
	struct dirent	*file;

	new_content = malloc(sizeof(char));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new_content[0] = '\0';
	cwd = getcwd(NULL, 0);
	directory = opendir(cwd);
	file = readdir(directory);
	while (file)
	{
		if (!ft_strcmp(content, "*") && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			if (new_content[0])
			{
				tmp = ft_strdup(new_content);
				if (!tmp)
					return (ft_putstr_fd(NO_SPACE, 2), NULL);
				free(new_content);
				new_content = ft_strjoin(tmp, " ");
				if (!new_content)
					return (ft_putstr_fd(NO_SPACE, 2), NULL);
				free(tmp);
			}
			tmp = ft_strdup(new_content);
			if (!tmp)
				return (ft_putstr_fd(NO_SPACE, 2), NULL);
			free(new_content);
			new_content = ft_strjoin(tmp, file->d_name);
			if (!new_content)
				return (ft_putstr_fd(NO_SPACE, 2), NULL);
			free(tmp);
		}
		file = readdir(directory);
	}
	free(cwd);
	closedir(directory);
	return (new_content);
}

// char	*expand_wildcards(char *content, int len)
char	*expand_wildcards(char *content, int len, int needs_expansion)
{
	char	*new_content;
	int		inside_d_quotes;
	int		inside_s_quotes;
	int		i;
	int		j;

	if (!needs_expansion)
	{
		new_content = malloc(sizeof(char) * (len + 1));
		if (!new_content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		new_content[len] = '\0';
	}
	inside_d_quotes = 0;
	inside_s_quotes = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		else if (content[i] == '"' && !inside_s_quotes)
			inside_d_quotes = !inside_d_quotes;
		else if (content[i] == '*' && !inside_s_quotes && !inside_d_quotes)
		{
			new_content = add_wildcard_content(content);
			return (new_content);
		}
		else if (!needs_expansion)
			new_content[++j] = content[i];
	}
	free(content);
	return (new_content);
}

int	needs_wildcard_expansion(char *content)
{
	int	inside_s_quotes;
	int	inside_d_quotes;
	int	i;

	inside_d_quotes = 0;
	inside_s_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		else if (content[i] == '"' && !inside_s_quotes)
			inside_d_quotes = !inside_d_quotes;
		else if (content[i] == '*' && !inside_s_quotes && !inside_d_quotes)
			return (1);
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
	// QUOTE REMOVAL
	// if (needs_quote_removal(expanded_content))
	// {
		expanded_len = quote_removal_str_len(expanded_content);
		expanded_content = remove_quotes_expansion(expanded_content, expanded_len);
		if (!expanded_content)
			return (free(expanded_content), NULL);
	// }
	printf("After quote removal -> %s\n", expanded_content);
	// PARAMETER EXPANSION
	// if (needs_parameter_expansion(expanded_content))
	// {
		expanded_len = parameter_expansion_str_len(msh, expanded_content);
		expanded_content = expand_parameter(msh, expanded_content, expanded_len);
		if (!expanded_content)
			return (free(expanded_content), NULL);
	// }
	printf("After parameter expansion -> %s\n", expanded_content);
	// WILDCARDS
		expanded_len = wildcards_str_len(expanded_content);
		printf("Wildcard length -> %d\n", expanded_len);
		expanded_content = expand_wildcards(expanded_content, expanded_len, needs_wildcard_expansion(expanded_content));
		if (!expanded_content)
			return (free(expanded_content), NULL);
	printf("After wildcard expansion -> %s\n", expanded_content);
	return (expanded_content);
}

int	str_len_no_quotes(char *content)
{
	int		real_len;
	int		inside_s_quotes;
	int		inside_d_quotes;
	int		i;

	real_len = 0;
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		else if (content[i] == '"' && !inside_s_quotes)
			inside_d_quotes = !inside_d_quotes;
		else
			real_len++;
	}
	return (real_len);
}

char	*set_no_quotes_content(char	*content, int real_len)
{
	char	*no_quote_content;
	int		inside_s_quotes;
	int		inside_d_quotes;
	int		i;
	int		j;

	no_quote_content = malloc(sizeof(char) * (real_len + 1));
	if (!no_quote_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	inside_s_quotes = 0;
	inside_d_quotes = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !inside_d_quotes)
			inside_s_quotes = !inside_s_quotes;
		else if (content[i] == '"' && !inside_s_quotes)
			inside_d_quotes = !inside_d_quotes;
		else
			no_quote_content[++j] = content[i];
	}
	no_quote_content[++j] = '\0';
	return (no_quote_content);
}

char	*remove_quotes(char *content)
{
	char	*quotes_removed;
	int		real_len;

	if (!needs_quote_removal(content))
		return (content);
	real_len = str_len_no_quotes(content);
	quotes_removed = set_no_quotes_content(content, real_len);
	if (!quotes_removed)
		return (NULL);
	free(content);
	return (quotes_removed);
}

t_redir_list	*expand_redirs(t_minishell *msh, t_command_table *command_table)
{
	t_redir_list	*tmp_redir;

	tmp_redir = command_table->redirs;
	while (tmp_redir)
	{
		//if (tmp_redir->type == HERE_DOC)
		//{
		//	set_here_doc_expansion(&tmp_redir);
		//	tmp_redir->here_doc_limiter = remove_quotes(tmp_redir->here_doc_limiter);
		//	if (!tmp_redir->here_doc_limiter)
		//		return (free_command_table(command_table), NULL);
		//}
		//else
		//{
			// if (need_expansion(tmp_redir->file) && expansion_inside_d_quotes(tmp_redir->file))
			// 	tmp_redir->ambiguous_redirect = 1;
		if (tmp_redir->type != HERE_DOC)
		{
			tmp_redir->file = expand_content(msh, tmp_redir->file);
			if (!tmp_redir->file)
				return (free_command_table(command_table), NULL);
		}
		//}
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
	free_command_table(command_table);
}
