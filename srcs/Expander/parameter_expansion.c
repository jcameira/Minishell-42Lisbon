/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:26:59 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 15:44:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	parameter_expansion_str_len_aux(char *content, int *quotes, int *i,
	int *real_len)
{
	if (content[*i] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		(*real_len)++;
		return ;
	}
	else if (content[*i] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		(*real_len)++;
		return ;
	}
	(*real_len)++;
}

int	parameter_expansion_str_len(t_minishell *msh, t_command_table *table,
	char *content)
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
			parameter_expansion_str_len_aux(content, quotes, &i, &real_len);
		else if (content[i] == '$' && quotes[D] && content[i + 1] == '"')
			real_len++;
		else if (content[i] == '$' && content[i + 1]
			&& (isenvchar(content[i + 1]) || content[i + 1] == '?')
			&& !quotes[S])
			real_len += get_env_variable_len(msh, table, content, &i);
		else
			real_len++;
	}
	return (real_len);
}

void	check_parameter_quotes(char *content, char **new_content, int *indexes,
	int *quotes)
{
	if (content[indexes[0]] == '\'' && !quotes[D])
	{
		quotes[S] = !quotes[S];
		(*new_content)[++indexes[1]] = content[indexes[0]];
		return ;
	}
	else if (content[indexes[0]] == '"' && !quotes[S])
	{
		quotes[D] = !quotes[D];
		(*new_content)[++indexes[1]] = content[indexes[0]];
		return ;
	}
	(*new_content)[++indexes[1]] = content[indexes[0]];
}

void	init_needed_variables(char *content, int (*quotes)[2],
	int (*indexes)[2], char *(*contents)[2])
{
	(*quotes)[S] = 0;
	(*quotes)[D] = 0;
	(*indexes)[0] = -1;
	(*indexes)[1] = -1;
	(*contents)[0] = content;
}

char	*expand_parameter(t_minishell *msh, t_command_table *table,
	char *content, int len)
{
	char	*new_content;
	char	*contents[2];
	int		quotes[2];
	int		indexes[2];

	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	init_needed_variables(content, &quotes, &indexes, &contents);
	contents[1] = new_content;
	while (contents[0][++indexes[0]])
	{
		if (contents[0][indexes[0]] == '\'' || contents[0][indexes[0]] == '"')
			check_parameter_quotes(contents[0], &new_content, indexes, quotes);
		else if (contents[0][indexes[0]] == '$' && quotes[D] && contents[0][indexes[0] + 1] == '"')
			contents[1][++indexes[1]] = contents[0][indexes[0]];
		else if (contents[0][indexes[0]] == '$' && contents[0][indexes[0] + 1]
			&& contents[0][indexes[0] + 1] != ' ' && !quotes[S])
			contents[1] = add_expanded_parameter(msh, table, contents, &indexes);
		else
			contents[1][++indexes[1]] = contents[0][indexes[0]];
	}
	contents[1][++indexes[1]] = '\0';
	free(contents[0]);
	return (contents[1]);
}
