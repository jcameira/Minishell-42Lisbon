/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:54:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 14:55:06 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <libft.h>

# define NO_SPACE "No more space left in device\n"

# define PRINT_NO_TYPE	"NO_TYPE"
# define PRINT_AND	"AND"
# define PRINT_OR	"OR"
# define PRINT_PIPE	"PIPE"
# define PRINT_LESSER	"LESSER"
# define PRINT_D_LESSER	"D_LESSER"
# define PRINT_GREATER	"GREATER"
# define PRINT_D_GREATER	"D_GREATER"
# define PRINT_SUBSHELL	"SUBSHELL"
# define PRINT_L_PARENTESIS	"L_PARENTESIS"
# define PRINT_R_PARENTESIS	"R_PARENTESIS"
# define PRINT_WORD	"WORD"

typedef enum s_tokens
{
	NO_TYPE = 0,
	AND,
	OR,
	PIPE,
	LESSER,
	D_LESSER,
	GREATER,
	D_GREATER,
	L_PARENTESIS,
	R_PARENTESIS,
	WORD,
	BAD_TOKEN,
	END
}				t_tokens;

typedef struct s_token_list
{
	int					token_type;
	char				*content;
	struct s_token_list	*next;
}				t_token_list;

typedef struct s_ast
{
	char			*type;
	char			*content;
	struct s_ast	*subshell_ast;
	struct s_ast	*left;
	struct s_ast	*right;
}				t_ast;

char	*set_ast_node_type(t_token_list *token_node);
char	*get_node_content(t_token_list *token_node);

#endif