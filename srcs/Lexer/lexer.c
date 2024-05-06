/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/06 19:48:38 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//tokens to be searched for in order of priority
//()
//&&
//||
//|
//<
//<<
//>
//>>

//->check for initial syntax errors in the line - DONE
//->if there is anything in parentesis open a subshell and send it there to be
//executed
//->wait for the subshell to finish execution before continuing
//->create an ast with these tokens
//->read through the entire line looking for one of these tokens, then assign
//it to a node and send everything to the left of the token to be veryfied on
//the left node, and send everything to the right of the token to be verified on
//the right node
//->handle the presence of parentesis by opening a subshell and sending
//everything inside the parentesis to the lexer of said subshell to start the
//parsing process
//->preferably handle quotes here, or at least any possible errors from unclosed
//quotes
//->when there is no other recognizible tokens to be found, assign the remaining
//words to a node and send the ast to the parser to finish parsing

#include <lexer.h>

void	lexer(char *line)
{
	bool	subshell;

	subshell = false;
	if (!check_odd_number_tokens(line, &subshell))
		return ;
	if (subshell)
		open_subshell(line);
}
