/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 00:06:02 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	clean_up_arg_arr(char ***content, int index)
{
	char	**new_content;
	int		arr_size;
	int		i;
	int		j;

	arr_size = array_size(*content);
	new_content = malloc(sizeof(char *) * arr_size);
	i = -1;
	j = -1;
	while ((*content)[++i])
		if (i != index)
			new_content[++j] = ft_strdup((*content)[i]);
	new_content[++j] = NULL;
	free_arr(*content);
	*content = new_content;
}

char	*continue_expand_content(char *expanded_content, char ***content,
	int *i, int original_i)
{
	int		expanded_len;

	expanded_len = wildcards_str_len(expanded_content);
	expanded_content = expand_wildcards(expanded_content, expanded_len,
			needs_wildcard_expansion(expanded_content));
	if (!expanded_content)
		return (free((*content)[*i]), NULL);
	expanded_len = quote_removal_str_len(expanded_content);
	expanded_content = remove_quotes_expansion(expanded_content, expanded_len);
	if (!expanded_content)
		return (free((*content)[*i]), NULL);
	free((*content)[original_i]);
	return (expanded_content);
}

char	*expand_content(t_minishell *msh, t_final_cmd_table *table,
	char ***content, int *i)
{
	char	*expanded_content;
	int		expanded_len;
	int		original_i;

	original_i = *i;
	expanded_content = ft_strdup((*content)[*i]);
	if (!expanded_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	expanded_len = parameter_expansion_str_len(msh, table, expanded_content);
	expanded_content = expand_parameter(msh, table, expanded_content,
			expanded_len);
	if (!expanded_content)
		return (free((*content)[*i]), NULL);
	if (!expanded_content[0])
	{
		clean_up_arg_arr(content, *i);
		(*i)--;
		return (free(expanded_content), NULL);
	}
	expanded_content = word_split(expanded_content, content, *i);
	if (!expanded_content)
		return (free((*content)[*i]), NULL);
	return (continue_expand_content(expanded_content, content, i, original_i));
}

void	expand_redirs(t_minishell *msh, t_final_cmd_table *command_table)
{
	char	**tmp_file;
	int		tmp;

	if (command_table->infile)
		command_table->ambiguous_redirect = set_ambiguous_redirect(
				msh, command_table, command_table->infile);
	if (command_table->infile && command_table->ambiguous_redirect)
		command_table->infile = remove_quotes_expansion(command_table->infile,
				quote_removal_str_len(command_table->infile));
	if (command_table->outfile)
		command_table->ambiguous_redirect = set_ambiguous_redirect(
				msh, command_table, command_table->outfile);
	if (command_table->outfile && command_table->ambiguous_redirect)
		command_table->outfile = remove_quotes_expansion(command_table->outfile,
				quote_removal_str_len(command_table->outfile));
	tmp_file = &command_table->infile;
	tmp = 0;
	if (!command_table->ambiguous_redirect && command_table->infile)
		command_table->infile = expand_content(msh, command_table,
				&tmp_file, &tmp);
	tmp_file = &command_table->outfile;
	if (!command_table->ambiguous_redirect && command_table->outfile)
		command_table->outfile = expand_content(msh, command_table,
				&tmp_file, &tmp);
}

int	expander(t_minishell *msh, t_final_cmd_table *command_table)
{
	t_final_cmd_table	*tmp_table;
	char				*tmp;
	int					i;

	tmp_table = command_table;
	if (tmp_table->simplecmd)
	{
		i = -1;
		while (tmp_table->simplecmd->arg_arr
			&& tmp_table->simplecmd->arg_arr[++i])
		{
			tmp = expand_content(msh,
					tmp_table, &tmp_table->simplecmd->arg_arr, &i);
			if (tmp)
				tmp_table->simplecmd->arg_arr[i] = ft_strdup(tmp);
			free(tmp);
		}
	}
	expand_redirs(msh, tmp_table);
	if (command_table->simplecmd && command_table->simplecmd->arg_arr)
		command_table->builtin = builtin_arr(
				command_table->simplecmd->arg_arr[0]);
	command_table->simplecmd->arg_nbr = array_size(
			command_table->simplecmd->arg_arr);
	return (1);
}
