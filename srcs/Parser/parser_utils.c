/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:59:16 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 17:21:54 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

char	*set_ast_node_type(t_token_list *token_node)
{
	if (!token_node)
		return (NULL);
	if (token_node->token_type == AND)
		return (PRINT_AND);
	if (token_node->token_type == OR)
		return (PRINT_OR);
	if (token_node->token_type == PIPE)
		return (PRINT_PIPE);
	if (token_node->token_type == LESSER)
		return (PRINT_LESSER);
	if (token_node->token_type == D_LESSER)
		return (PRINT_D_LESSER);
	if (token_node->token_type == GREATER)
		return (PRINT_GREATER);
	if (token_node->token_type == D_GREATER)
		return (PRINT_D_GREATER);
	if (token_node->token_type == L_PARENTESIS)
		return (PRINT_SUBSHELL);
	if (token_node->token_type == WORD)
		return (PRINT_WORD);
}

char	*get_node_content(t_token_list *token_node)
{
	if (!token_node)
		return (NULL);
	return (ft_strdup(token_node->content));
}

char	*get_subshell_content(t_token_list *token_node)
{
	t_token_list	*tmp_node;
	char			*content;
	char			*tmp_content;
	int				parentesis;

	tmp_node = token_node->next;
	content = NULL;
	parentesis = 1;
	while (tmp_node && parentesis)
	{
		if (tmp_node->token_type == L_PARENTESIS)
			parentesis++;
		if (tmp_node->token_type == R_PARENTESIS)
			parentesis--;
		tmp_content = content;
		free(content);
		content = ft_strjoin(tmp_content, tmp_node->content);
		if (!content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		tmp_node = tmp_node->next;
	}
	return (content);
}

int	check_full_cmd_line_subshell(t_token_list *token_list)
{
	t_token_list	*tmp;
	int				parentesis;

	if (token_list->token_type != L_PARENTESIS)
		return (0);
	tmp = token_list->next;
	parentesis = 1;
	while (tmp)
	{
		if (tmp->token_type == L_PARENTESIS)
			parentesis++;
		if (tmp->token_type == R_PARENTESIS)
			parentesis--;
		tmp = tmp->next;
	}
	if (parentesis)
		return (0);
	return (1);
}

t_token_list	*trim_parentesis_nodes(t_token_list *token_list)
{
	t_token_list	tmp;

	node = new_ast_node(token_list);
	
}