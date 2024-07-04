/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 04:35:48 by jcameira         ###   ########.fr       */
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

char	*get_env_value(t_minishell *msh, char *env_name)
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
	return (env_name);
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
	int	quotes[2];
	int	i;

	quotes[D] = 0;
	quotes[S] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '"' && !quotes[S])
			quotes[S] = !quotes[D];
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		if (content[i] == '$' && !quotes[S])
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

void	quote_removal_str_len_aux(char *content, int *quotes, int i,
	int *real_len)
{
	if (content[i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, i, '\'', QUOTES))
			real_len += len_inside_quotes(content, &i, '\'');
	}
	else if (content[i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, i, '"', QUOTES))
			real_len += len_inside_quotes(content, &i, '"');
	}
}

int	quote_removal_str_len(char *content)
{
	int	real_len;
	int	quotes[2];
	int	i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' || content[i] == '"')
			quote_removal_str_len_aux(content, quotes, i, &real_len);
		else
			real_len++;
	}
	return (real_len);
}

void	add_content_inside_quotes(char *content, char *new_content, int *i,
	int *j)
{
	char	quote_type;

	quote_type = content[*i];
	new_content[++(*j)] = content[*i];
	while (content[++(*i)] != quote_type)
		new_content[++(*j)] = content[*i];
	new_content[++(*j)] = content[*i];
}

void	quote_removal_expansion_aux(char *content, char *new_content,
	int *indexes, int *quotes)
{
	if (content[indexes[0]] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, indexes[0], '\'',
				QUOTES))
			add_content_inside_quotes(content, new_content, &indexes[0],
				&indexes[1]);
	}
	else if (content[indexes[0]] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, indexes[0], '"',
				QUOTES))
			add_content_inside_quotes(content, new_content, &indexes[0],
				&indexes[1]);	
	}
}

char	*remove_quotes_expansion(char *content, int len)
{
	char	*new_content;
	int		quotes[2];
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	quotes[S] = 0;
	quotes[D] = 0;
	indexes[0] = -1;
	indexes[1] = -1;
	while (content[++indexes[0]])
	{
		if (content[indexes[0]] == '\'' || content[indexes[0]] == '"')
			quote_removal_expansion_aux(content, new_content, indexes, quotes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
}

void	parameter_expansion_str_len_aux(char *content, int *quotes, int i,
	int *real_len)
{
	int	had_expansion_in_quotes;

	had_expansion_in_quotes = 0;
	if (content[i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, i, '\'',
				PARAMETERS))
			real_len += len_inside_quotes(content, &i, '\'');
	}
	else if (content[i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, i, '"',
				PARAMETERS))
		{
			had_expansion_in_quotes = !had_expansion_in_quotes;
			real_len++;
		}
		if (!quotes[D] && had_expansion_in_quotes)
		{
			had_expansion_in_quotes = !had_expansion_in_quotes;
			real_len++;
		}
	}
}

int	parameter_expansion_str_len(t_minishell *msh, char *content)
{
	int	real_len;
	int	quotes[2];
	int	i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' || content[i] == '"')
			parameter_expansion_str_len_aux(content, quotes, i, &real_len);
		else if (content[i] == '$' && content[i + 1]
			&& isenvchar(content[i + 1]) && !quotes[S])
			real_len += get_env_variable_len(msh, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

char	*add_expanded_parameter(t_minishell *msh, char *content,
	char *new_content, int *indexes)
{
	char	*env_name;
	char	*env_value;

	env_name = get_env_name(content, &indexes[0]);
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
	new_content = add_expanded_var(env_value, new_content, &indexes[1]);
	free(env_name);
	free(env_value);
	return (new_content);
}

void	update_new_content(char *content, char **new_content, int *indexes,
	int *had_expansion_in_quotes)
{
	(*had_expansion_in_quotes) = !(*had_expansion_in_quotes);
	(*new_content)[++indexes[1]] = content[indexes[0]];
}

void	check_parameter_quotes(char *content, char **new_content, int *indexes,
	int *quotes)
{
	int		had_expansion_in_quotes;

	had_expansion_in_quotes = 0;
	if (content[indexes[0]] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		if (quotes[S] && expansion_inside_quotes(content, indexes[0], '\'',
				PARAMETERS))
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
		if (!quotes[S] && had_expansion_in_quotes)
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
	}
	else if (content[indexes[0]] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		if (quotes[D] && expansion_inside_quotes(content, indexes[0], '"',
				PARAMETERS))
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
		if (!quotes[D] && had_expansion_in_quotes)
			update_new_content(content, new_content, indexes,
				&had_expansion_in_quotes);
	}
}

char	*expand_parameter(t_minishell *msh, char *content, int len)
{
	char	*new_content;
	int		quotes[2];
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	quotes[S] = 0;
	quotes[D] = 0;
	indexes[0] = -1;
	indexes[1] = -1;
	while (content[++indexes[0]])
	{
		if (content[indexes[0]] == '\'' || content[indexes[0]] == '"')
			check_parameter_quotes(content, &new_content, indexes, quotes);
		if (content[indexes[0]] == '$' && content[indexes[0] + 1] && !quotes[S])
			new_content = add_expanded_parameter(msh, content, new_content,
					indexes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
	free(content);
	return (new_content);
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

char	*expand_parameter_inside_here_doc(t_minishell *msh,
	char *content, int len)
{
	char	*new_content;
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	indexes[0] = -1;
	indexes[1] = -1;
	while (content[++indexes[0]])
	{
		if (content[indexes[0]] == '$' && content[indexes[0] + 1])
			new_content = add_expanded_parameter(msh, content, new_content,
					indexes);
		else
			new_content[++indexes[1]] = content[indexes[0]];
	}
	new_content[++indexes[1]] = '\0';
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
	expanded_content = expand_parameter_inside_here_doc(msh, expanded_content,
			expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	return (expanded_content);
}

int	match_wildcard_pattern(char *pattern, char *file)
{
	if (!(*pattern) && !(*file))
		return (1);
	if (*pattern == '*')
		return (match_wildcard_pattern(pattern + 1, file)
			|| (*file && match_wildcard_pattern(pattern, file + 1)));
	if (*pattern == *file)
		return (match_wildcard_pattern(pattern + 1, file + 1));
	return (0);
}

DIR	*get_directory_info(DIR *directory, char **new_content,
	struct dirent **file, int flag)
{
	char	*cwd;

	if (flag)
	{
		*new_content = malloc(sizeof(char));
		if (!(*new_content))
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		(*new_content)[0] = '\0';
	}
	cwd = getcwd(NULL, 0);
	directory = opendir(cwd);
	free(cwd);
	*file = readdir(directory);
	return (directory);
}

int	get_wildcards_len(char *content)
{
	int				real_len;
	DIR				*directory;
	struct dirent	*file;

	real_len = 0;
	directory = NULL;
	directory = get_directory_info(directory, NULL, &file, 0);
	while (file)
	{
		if (!ft_strcmp(content, "*") && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			if (real_len > 0)
				real_len++;
			real_len += ft_strlen(file->d_name);
		}
		file = readdir(directory);
	}
	closedir(directory);
	return (real_len);
}

int	wildcards_str_len(char *content)
{
	int	real_len;
	int	quotes[2];
	int	i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
		{
			real_len += get_wildcards_len(content);
			return (real_len);
		}
		else
			real_len++;
	}
	return (real_len);
}

char	*append_more_wildcard_content(char *new_content, struct dirent *file)
{
	char			*tmp;

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
	return (new_content);
}

char	*add_wildcard_content(char *content)
{
	char			*new_content;
	DIR				*directory;
	struct dirent	*file;

	directory = NULL;
	directory = get_directory_info(directory, &new_content, &file, 1);
	if (!new_content)
		return (NULL);
	while (file)
	{
		if (!ft_strcmp(content, "*") && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			new_content = append_more_wildcard_content(new_content, file);
			if (!new_content)
				return (NULL);
		}
		file = readdir(directory);
	}
	closedir(directory);
	return (new_content);
}

char	*expand_wildcards_aux(char *new_content, char *content,
	int needs_expansion)
{
	int		quotes[2];
	int		i;
	int		j;

	quotes[D] = 0;
	quotes[S] = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
		{
			new_content = add_wildcard_content(content);
			break ;
		}
		else if (!needs_expansion)
			new_content[++j] = content[i];
	}
	free(content);
	return (new_content);
}

char	*expand_wildcards(char *content, int len, int needs_expansion)
{
	char	*new_content;

	if (!needs_expansion)
	{
		new_content = malloc(sizeof(char) * (len + 1));
		if (!new_content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		new_content[len] = '\0';
	}
	new_content = expand_wildcards_aux(new_content, content, needs_expansion);
	if (!new_content)
		return (NULL);
	return (new_content);
}

int	needs_wildcard_expansion(char *content)
{
	int	quotes[2];
	int	i;

	quotes[D] = 0;
	quotes[S] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
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
	printf("Before expansions -> %s\n", expanded_content);
	expanded_len = quote_removal_str_len(expanded_content);
	expanded_content = remove_quotes_expansion(expanded_content, expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	printf("After quotes removal -> %s\n", expanded_content);
	expanded_len = parameter_expansion_str_len(msh, expanded_content);
	expanded_content = expand_parameter(msh, expanded_content, expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	printf("After parameter expansion -> %s\n", expanded_content);
	expanded_len = wildcards_str_len(expanded_content);
	expanded_content = expand_wildcards(expanded_content, expanded_len,
			needs_wildcard_expansion(expanded_content));
	if (!expanded_content)
		return (free(expanded_content), NULL);
	printf("After wildcard expansion -> %s\n", expanded_content);
	return (expanded_content);
}

int	str_len_no_quotes(char *content)
{
	int		real_len;
	int		quotes[2];
	int		i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else
			real_len++;
	}
	return (real_len);
}

char	*set_no_quotes_content(char	*content, int real_len)
{
	char	*no_quote_content;
	int		quotes[2];
	int		i;
	int		j;

	no_quote_content = malloc(sizeof(char) * (real_len + 1));
	if (!no_quote_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
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
		if (tmp_redir->type != HERE_DOC)
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
