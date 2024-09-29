/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expansions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:30:01 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/28 23:22:24 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

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

// int	parameter_expansion_here_doc_str_len(t_minishell *msh,
// 	t_command_table *table, char *content)
// {
// 	int	real_len;
// 	int	i;

// 	real_len = 0;
// 	i = -1;
// 	while (content[++i])
// 	{
// 		if (content[i] == '$' && content[i + 1] && isenvchar(content[i + 1]))
// 			real_len += get_env_variable_len(msh, table, content, &i);
// 		else
// 			real_len++;
// 	}
// 	return (real_len);
// }

int	parameter_expansion_here_doc_str_len(t_minishell *msh,
	t_final_cmd_table *table, char *content)
{
	int	real_len;
	int	i;

	real_len = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '$' && content[i + 1] && isenvchar(content[i + 1]))
			real_len += get_env_variable_len(msh, table, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

char	*expand_parameter_inside_here_doc(t_minishell *msh,
	char *content, int len)
{
	char	*new_content;
	char	*contents[2];
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	indexes[0] = -1;
	indexes[1] = -1;
	contents[0] = content;
	contents[1] = new_content;
	while (contents[0][++indexes[0]])
	{
		if (contents[0][indexes[0]] == '$' && contents[0][indexes[0] + 1])
			contents[1] = add_expanded_parameter(msh, NULL, contents,
					&indexes);
		else
			contents[1][++indexes[1]] = contents[0][indexes[0]];
	}
	contents[1][++indexes[1]] = '\0';
	free(contents[0]);
	return (contents[1]);
}

// char	*expansion_inside_here_doc(t_minishell *msh, t_command_table *table,
// 	char *content, int flag)
// {
// 	char	*expanded_content;
// 	int		expanded_len;

// 	if (flag)
// 		return (content);
// 	expanded_content = ft_strdup(content);
// 	if (!expanded_content)
// 		return (ft_putstr_fd(NO_SPACE, 2), NULL);
// 	free(content);
// 	expanded_len = parameter_expansion_here_doc_str_len(msh, table,
// 			expanded_content);
// 	expanded_content = expand_parameter_inside_here_doc(msh, expanded_content,
// 			expanded_len);
// 	if (!expanded_content)
// 		return (free(expanded_content), NULL);
// 	return (expanded_content);
// }

char	*expansion_inside_here_doc(t_minishell *msh, t_final_cmd_table *table,
	char *content, int flag)
{
	char	*expanded_content;
	int		expanded_len;

	if (flag)
		return (content);
	expanded_content = ft_strdup(content);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free(content);
	expanded_len = parameter_expansion_here_doc_str_len(msh, table,
			expanded_content);
	expanded_content = expand_parameter_inside_here_doc(msh, expanded_content,
			expanded_len);
	if (!expanded_content)
		return (free(expanded_content), NULL);
	return (expanded_content);
}
