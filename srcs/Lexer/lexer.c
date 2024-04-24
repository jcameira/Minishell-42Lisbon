/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/24 02:26:27 by jcameira         ###   ########.fr       */
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

//->create an ast with these tokens
//->read through the entire line looking for one of these tokens, then assign
//it to a node and send everything to the left of the token to be veryfied on
//the left node, and send everything to the right of the token to be verified on
//the right node
//->preferably handle quotes here, or at least any possible errors from unclosed
//quotes
//->handle the presence of parentesis by opening a subshell and executing what's
//inside parentesis in said subshell
//->when there is no other recognizible tokens to be found, assign the remaining
//words to a node and send the ast to the parser to finish parsing