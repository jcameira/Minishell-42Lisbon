/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:59:16 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/20 18:24:58 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_ast_token_type	set_ast_node_type(t_token_list *token_node)
{
	if (!token_node)
		return (NO_NODE);
	else if (token_node->token_type == AND
		|| token_node->token_type == OR)
		return (AND_OR_SEQUENCE);
	else if (token_node->token_type == PIPE)
		return (PIPE_SEQUENCE);
	else if (token_node->token_type == LESSER
		|| token_node->token_type == D_LESSER
		|| token_node->token_type == GREATER
		|| token_node->token_type == D_GREATER)
		return (REDIRECTION);
	else if (token_node->token_type == L_PARENTESIS)
		return (SUBSHELL);
	else if (token_node->token_type == WORD)
		return (SIMPLE_COMMAND);
	else
		return (BAD_TYPE);
}

char	*get_node_content(t_token_list *token_node)
{
	if (!token_node)
		return (NULL);
	return (ft_strdup(token_node->content));
}

static char	*add_more_content(char *content, t_token_list *node, int par)
{
	char	*tmp_content;

	tmp_content = ft_strdup(content);
	free(content);
	if (!tmp_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	content = ft_strjoin(tmp_content, node->content);
	free(tmp_content);
	if (!content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	if (par)
	{
		tmp_content = ft_strdup(content);
		free(content);
		if (!tmp_content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		content = ft_strjoin(tmp_content, " ");
		free(tmp_content);
		if (!content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	return (content);
}

char	*get_subshell_content(t_token_list *token_node)
{
	t_token_list	*tmp_node;
	char			*content;
	int				parentesis;

	tmp_node = token_node->next;
	content = malloc(sizeof(char));
	if (!content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	content[0] = '\0';
	parentesis = 1;
	while (tmp_node && parentesis)
	{
		if (tmp_node->token_type == L_PARENTESIS)
			parentesis++;
		if (tmp_node->token_type == R_PARENTESIS)
			parentesis--;
		if (parentesis)
		{
			content = add_more_content(content, tmp_node, parentesis);
			if (!content)
				return (NULL);
		}
		tmp_node = tmp_node->next;
	}
	return (content);
}

char	*get_simple_command_content(t_token_list *token_node)
{
	t_token_list	*tmp_node;
	char			*content;

	tmp_node = token_node;
	content = malloc(sizeof(char));
	if (!content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	content[0] = '\0';
	while (tmp_node && tmp_node->token_type == WORD)
	{
		if (tmp_node->next)
			content = add_more_content(content, tmp_node, 1);
		else
			content = add_more_content(content, tmp_node, 0);
		tmp_node = tmp_node->next;
	}
	return (content);
}
