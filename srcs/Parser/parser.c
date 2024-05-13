/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 17:20:12 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->use the complete ast to parse the complete command table with all simple
//commands and redirections
//->carefull with the redirections, only one redirection for infile and one
//redirection for outfile should be available, this includes redirections for
//here documents that are in any case a redirection for infile, so if we read
//multiple in or out redirections we should update the redirection in the
//command table, for this if we read the ast I think reading from leaf to
//root and from left to right the best way, this way if we constantly update
//the in and out redirection in the end we will have the last written
//redirections which are the correct ones
//->check for any possible errors coming from bad command lines (this could and
//probably should be checked only in the lexer)
//->send the complete command table to the expander

#include <parser.h>

void	parser(t_token_list *token_list)
{
	t_ast	*node;

	if (check_full_cmd_line_subshell(token_list))
		node = new_ast_node(token_list);
}
