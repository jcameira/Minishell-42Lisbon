/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_errors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:52:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/22 21:20:04 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int	check_if_only_subshell_inside_subshell(t_token_list *token_list)
{
	int	parentesis;

	parentesis = 0;
	while (token_list->token_type == L_PARENTESIS)
	{
		parentesis++;
		token_list = token_list->next;
	}
	if (parentesis == 1)
		return (0);
	while (token_list->token_type != R_PARENTESIS)
		token_list = token_list->next;
	while (token_list && token_list->token_type == R_PARENTESIS)
	{
		parentesis--;
		token_list = token_list->next;
	}
	return (parentesis == 0);
}

int	check_and_or_syntax_errors(t_token_list *token_list, t_token_list *previous)
{
	return ((token_list->token_type == AND || token_list->token_type == OR)
		&& (!previous || !token_list->next
			|| (token_list->next->token_type != WORD
				&& token_list->next->token_type != L_PARENTESIS)));
}

int	check_pipe_syntax_errors(t_token_list *token_list, t_token_list *previous)
{
	return (token_list->token_type == PIPE && (!previous
			|| !token_list->next || (token_list->next->token_type != WORD
				&& token_list->next->token_type != L_PARENTESIS)));
}

int	check_subshell_syntax_errors(t_token_list *token_list)
{
	if (token_list->token_type == L_PARENTESIS
		&& (token_list->next->token_type == R_PARENTESIS
			|| check_if_only_subshell_inside_subshell(token_list)))
		return (1);
	else if (token_list->token_type == R_PARENTESIS)
	{
		while (token_list && token_list->token_type == R_PARENTESIS)
			token_list = token_list->next;
		if (!token_list)
			return (0);
		else if (token_list->token_type == L_PARENTESIS
			|| token_list->token_type == WORD)
			return (1);
	}
	return (0);
}

int	check_redirection_syntax_errors(t_token_list *token_list)
{
	return ((token_list->token_type == LESSER
			|| token_list->token_type == D_LESSER
			|| token_list->token_type == GREATER
			|| token_list->token_type == D_GREATER) && (!token_list->next
			|| token_list->next->token_type != WORD));
}
