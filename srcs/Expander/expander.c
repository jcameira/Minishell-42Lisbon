/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/06/19 18:58:43 by jcameira         ###   ########.fr       */
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
	return (ft_isalnum(c) || c == 137);
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
	env_value = ft_substr(tmp_envp[i], env_name_len,
		ft_strlen(&tmp_envp[i][env_name_len]));
	if (!env_value)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (env_value);
}

char	*get_env_name(t_minishell *msh, char *content, int *i)
{
	int		env_name_start;
	char	*env_name;

	env_name_start = ++(*i);
	while (content[*i] && isenvchar(content[*i]))
		(*i)++;
	env_name = ft_substr(content, env_name_start, *i);
	if (!env_name)
		return (ft_putstr_fd(NO_SPACE, 2), -1);
	return(env_name);
}

int	get_env_variable_len(t_minishell *msh, char *content, int *i)
{
	int		env_len;
	char	*env_name;
	char	*env_value;
	

	env_name = get_env_name(msh, content, i);
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

int	expanded_str_len(t_minishell *msh, char *content)
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
		else if (content[i] == '$' && content[i + 1] && !inside_s_quotes)
			real_len += get_env_variable_len(msh, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

char	*add_expanded_var(t_minishell *msh, char *content, int *i, int *j)
{

}

char	*set_expanded_content(t_minishell *msh, char *content, int expanded_len)
{
	char	*expanded_content;
	int		inside_s_quotes;
	int		inside_d_quotes;
	int		i;
	int		j;

	expanded_content = malloc(sizeof(char) * expanded_len);
	if (!expanded_content)
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
		else if (content[i] == '$' && content[i + 1] && !inside_s_quotes)
			expanded_content = add_expanded_var(msh, content, &i, &j);
		else
			expanded_content[++j] = content[i];
	}
	expanded_content[++j] = '\0';
	return (expanded_content);
}

char	*expand_content(t_minishell *msh, char *content)
{
	char	*expanded_content;
	int		inside_single_quotes;
	int		expanded_len;

	if (!need_expansion(content))
		return (content);
	expanded_len = expanded_str_len(msh, content);
	expanded_content = set_expanded_content(msh, content, expanded_content);
	if (!expanded_content)
		return (NULL);
	free(content);
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
	//executor(msh, command_table);
}
