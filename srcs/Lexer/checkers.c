/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:01:25 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 17:16:13 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int	validate_odd_number_tokens(int s_q, int d_q, int o_p, int c_p)
{
	if (s_q % 2 != 0)
	{
		ft_putstr_fd(UNCLOSED_S_QUOTES, 2);
		return (0);
	}
	if (d_q % 2 != 0)
	{
		ft_putstr_fd(UNCLOSED_D_QUOTES, 2);
		return (0);
	}
	if (o_p != c_p)
	{
		ft_putstr_fd(UNCLOSED_PARENTESIS, 2);
		return (0);
	}
	return (1);
}

int	check_odd_number_tokens(char *line)
{
	int	i;
	int	s_quotes;
	int	d_quotes;
	int	o_par;
	int	c_par;

	i = -1;
	s_quotes = 0;
	d_quotes = 0;
	o_par = 0;
	c_par = 0;
	while (line[++i] && c_par <= o_par)
	{
		if (line[i] == '\'' && d_quotes % 2 == 0)
			s_quotes++;
		else if (line[i] == '"' && s_quotes % 2 == 0)
			d_quotes++;
		else if (line[i] == '(' && s_quotes % 2 == 0 && d_quotes % 2 == 0)
			o_par++;
		else if (line[i] == ')' && s_quotes % 2 == 0 && d_quotes % 2 == 0)
			c_par++;
	}
	return (validate_odd_number_tokens(s_quotes, d_quotes, o_par, c_par));
}

int	is_operator_token(char	*c)
{
	return (!ft_strncmp(c, "&&", 2) || *c == '|' || *c == '<'
		|| *c == '>' || *c == '(' || *c == ')');
}

int	choose_syntax_error_message(t_token_list *token_list,
	t_token_list *previous)
{
	(void)previous;
	if (!token_list->next)
		return (ft_putstr_fd(NEWLINE_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == AND)
		return (ft_putstr_fd(AND_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == OR)
		return (ft_putstr_fd(OR_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == PIPE)
		return (ft_putstr_fd(PIPE_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == LESSER)
		return (ft_putstr_fd(INFILE_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == D_LESSER)
		return (ft_putstr_fd(HERE_DOC_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == GREATER)
		return (ft_putstr_fd(OUTFILE_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == D_GREATER)
		return (ft_putstr_fd(APPEND_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == L_PARENTESIS)
		return (ft_putstr_fd(OPEN_PARENTESIS_PARSE_ERROR, 2), 1);
	if (token_list->next->token_type == R_PARENTESIS)
		return (ft_putstr_fd(CLOSE_PARENTESIS_PARSE_ERROR, 2), 1);
	if (previous->token_type == WORD)
		return (ft_putstr_fd(OPEN_PARENTESIS_PARSE_ERROR, 2), 1);
	return (0);
}

int	check_syntax_errors(t_token_list *token_list)
{
	t_token_list	*previous;

	if (!token_list)
		return (0);
	previous = NULL;
	while (token_list)
	{
		if (check_and_or_syntax_errors(token_list, previous))
			return (1);
		else if (check_pipe_syntax_errors(token_list, previous))
			return (1);
		else if (check_subshell_syntax_errors(token_list, previous))
			return (1);
		else if (check_redirection_syntax_errors(token_list, previous))
			return (1);
		previous = token_list;
		token_list = token_list->next;
	}
	return (0);
}
