/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:01:25 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/03 16:16:22 by jcameira         ###   ########.fr       */
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

int	check_odd_number_tokens(char *line, bool *subshell)
{
	int	i;
	int	s_quotes;
	int	d_quotes;
	int	open_par;
	int	close_par;

	i = -1;
	s_quotes = 0;
	d_quotes = 0;
	open_par = 0;
	close_par = 0;
	while (line[++i])
	{
		if (line[i] == '\'')
			s_quotes++;
		else if (line[i] == '\"')
			d_quotes++;
		else if (line[i] == '(')
			open_par++;
		else if (line[i] == ')')
			close_par++;
	}
	if (open_par > 0)
		*subshell = true;
	return (validate_odd_number_tokens(s_quotes, d_quotes, open_par, close_par));
}
