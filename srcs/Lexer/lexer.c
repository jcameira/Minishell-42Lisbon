/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/28 03:46:32 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

int	lexer(t_minishell *msh, char *line)
{
	t_token_list	*token_list;

	ignore_signals_init();
	if (!check_odd_number_tokens(line))
		return (-1);
	token_list = get_initial_list(line);
	if (!token_list)
		return (-1);
	token_list = refine_list(token_list);
	if (!token_list)
		return (-1);
	token_list = set_token_list_types(token_list);
	if (check_syntax_errors(token_list))
		return (free_token_list(token_list), 2);
	return (parser(msh, token_list));
}
