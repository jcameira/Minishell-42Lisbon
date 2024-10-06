/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_errors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:52:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/06 12:14:06 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int	check_if_only_subshell_inside_subshell(t_token_list *token_list)
{
	int		parentesis;

	parentesis = 1;
	token_list = token_list->next;
	while (token_list)
	{
		if (token_list->token_type == L_PARENTESIS)
			parentesis++;
		if (token_list->token_type == R_PARENTESIS)
			parentesis--;
		if (token_list->token_type != L_PARENTESIS
			&& token_list->token_type != R_PARENTESIS && parentesis == 1)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	check_and_or_syntax_errors(t_token_list *token_list,
	t_token_list *previous)
{
	if ((token_list->token_type == AND || token_list->token_type == OR)
		&& (!previous || !token_list->next
			|| (token_list->next->token_type != WORD
				&& token_list->next->token_type != L_PARENTESIS
				&& token_list->next->token_type != GREATER
				&& token_list->next->token_type != LESSER
				&& token_list->next->token_type != D_GREATER
				&& token_list->next->token_type != D_LESSER)))
	{
		if (token_list->token_type == AND)
			return (ft_putstr_fd(AND_PARSE_ERROR, 2), 1);
		if (token_list->token_type == OR)
			return (ft_putstr_fd(OR_PARSE_ERROR, 2), 1);
	}
	return (0);
}

int	check_pipe_syntax_errors(t_token_list *token_list, t_token_list *previous)
{
	if (token_list->token_type == PIPE && (!previous
			|| !token_list->next || (token_list->next->token_type != WORD
				&& token_list->next->token_type != L_PARENTESIS
				&& token_list->next->token_type != GREATER
				&& token_list->next->token_type != LESSER
				&& token_list->next->token_type != D_GREATER
				&& token_list->next->token_type != D_LESSER)))
		return (ft_putstr_fd(PIPE_PARSE_ERROR, 2), 1);
	return (0);
}

int	check_subshell_syntax_errors(t_token_list *token_list,
	t_token_list *previous)
{
	if (token_list->token_type == L_PARENTESIS
		&& ((previous && previous->token_type == WORD)
			|| token_list->next->token_type == R_PARENTESIS
			|| check_if_only_subshell_inside_subshell(token_list)))
		return (choose_syntax_error_message(token_list, previous));
	else if (token_list->token_type == R_PARENTESIS)
	{
		while (token_list && token_list->token_type == R_PARENTESIS)
			token_list = token_list->next;
		if (!token_list)
			return (0);
		else if (token_list->token_type == L_PARENTESIS
			|| token_list->token_type == WORD)
			return (choose_syntax_error_message(token_list, previous));
	}
	return (0);
}

int	check_redirection_syntax_errors(t_token_list *token_list,
	t_token_list *previous)
{
	if ((token_list->token_type == LESSER
			|| token_list->token_type == D_LESSER
			|| token_list->token_type == GREATER
			|| token_list->token_type == D_GREATER) && (!token_list->next
			|| token_list->next->token_type != WORD))
	{
		if (token_list->token_type == LESSER)
			return (choose_syntax_error_message(token_list, previous));
		if (token_list->token_type == D_LESSER)
			return (choose_syntax_error_message(token_list, previous));
		if (token_list->token_type == GREATER)
			return (choose_syntax_error_message(token_list, previous));
		if (token_list->token_type == D_GREATER)
			return (choose_syntax_error_message(token_list, previous));
	}
	return (0);
}
