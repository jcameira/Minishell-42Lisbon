/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/10 15:55:48 by jcameira         ###   ########.fr       */
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

void	print_list(t_token_list *token_list)
{
	while (token_list)
	{
		if (token_list->token_type == AND)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_AND, token_list->content);
		else if (token_list->token_type == OR)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_OR, token_list->content);
		else if (token_list->token_type == PIPE)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_PIPE, token_list->content);
		else if (token_list->token_type == LESSER)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_LESSER, token_list->content);
		else if (token_list->token_type == D_LESSER)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == GREATER)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_GREATER, token_list->content);
		else if (token_list->token_type == D_GREATER)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == L_PARENTESIS)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_L_PARENTESIS, token_list->content);
		else if (token_list->token_type == R_PARENTESIS)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_R_PARENTESIS, token_list->content);
		else if (token_list->token_type == WORD)
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_WORD, token_list->content);
		else
			printf("TOKEN TYPE = %s TOKEN DATA = %s\n", PRINT_BAD_TOKEN, token_list->content);
		token_list = token_list->next;
	}
}

void	lexer(char *line)
{
	t_token_list	*token_list;
	t_token_list	*tmp;
	bool			subshell;

	subshell = false;
	if (!check_odd_number_tokens(line, &subshell))
		return ;
	// if (subshell)
	// 	open_subshell(line);
	// while (line)
	// 	add_new_token(&token_list, new_token(get_token_type(get_next_token(line))));
	
	token_list = get_initial_list(line);
	token_list = refine_list(&token_list);
	tmp = token_list;
	while (token_list)
	{
		token_list->token_type = get_token_type(token_list->content);
		//printf("TOKEN TYPE = %d TOKEN DATA = %s\n", token_list->token_type, token_list->content);
		token_list = token_list->next;
	}
	token_list = tmp;
	print_list(token_list);
}
