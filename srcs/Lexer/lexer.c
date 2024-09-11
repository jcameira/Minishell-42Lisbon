/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/11 15:26:08 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

void	print_list(t_token_list *token_list)
{
	while (token_list)
	{
		if (token_list->token_type == AND)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_AND, token_list->content);
		else if (token_list->token_type == OR)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_OR, token_list->content);
		else if (token_list->token_type == PIPE)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_PIPE, token_list->content);
		else if (token_list->token_type == LESSER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_LESSER, token_list->content);
		else if (token_list->token_type == D_LESSER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == GREATER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_GREATER, token_list->content);
		else if (token_list->token_type == D_GREATER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == L_PARENTESIS)
			printf("TOKEN TYPE = %s 	TOKEN DATA = %s\n", PRINT_L_PARENTESIS, token_list->content);
		else if (token_list->token_type == R_PARENTESIS)
			printf("TOKEN TYPE = %s 	TOKEN DATA = %s\n", PRINT_R_PARENTESIS, token_list->content);
		else if (token_list->token_type == WORD)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_WORD, token_list->content);
		else
			printf("TOKEN TYPE = %s 		TOKEN DATA = \n", PRINT_BAD_TOKEN);
		token_list = token_list->next;
	}
}

int	lexer(t_minishell *msh, char *line)
{
	t_token_list	*token_list;

	if (!check_odd_number_tokens(line))
		return (-1);
	token_list = get_initial_list(line);
	if (!token_list)
		return (-1);
	token_list = refine_list(token_list);
	if (!token_list)
		return (-1);
	token_list = set_token_list_types(token_list);
	//print_list(token_list);
	if (check_syntax_errors(token_list))
		return (free_token_list(token_list), -1);
	return(parser(msh, token_list));
}
