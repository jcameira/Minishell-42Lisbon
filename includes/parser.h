/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:54:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/16 14:11:46 by jcameira         ###   ########.fr       */
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

typedef enum s_ast_node_type
{
	BAD_TYPE = 0,
	AND_OR_SEQUENCE,
	PIPE_SEQUENCE,
	SUBSHELL,
	REDIRECTION,
	SIMPLE_COMMAND
}				t_ast_token_type;

typedef enum s_token_list_type
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
	BAD_TOKEN
}				t_token_list_type;

typedef struct s_token_list
{
	t_token_list_type	token_type;
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

char			*set_ast_node_type(t_token_list *token_node);
char			*get_node_content(t_token_list *token_node);
char			*get_subshell_content(t_token_list *token_node);
char			*get_simple_command_content(t_token_list *token_node);
int				check_for_subshell(t_token_list *token_list);
int				check_for_node(t_token_list *token_list, t_ast_token_type type);
void			trim_parentesis_nodes(t_token_list **token_list);
t_ast			*new_ast_node(t_token_list *token_node);
void			free_token_list_node(t_token_list **node);
void			print_list(t_token_list *token_list);
void			free_token_list(t_token_list *list);
t_ast			*add_ast_node(t_token_list **token_list);
t_ast			*new_regular_node(t_token_list **token_list, t_ast_token_type type);
t_ast			*new_subshell_node(t_token_list **token_list);
t_ast			*new_simple_command_node(t_token_list **token_list);
void			skip_subshell(t_token_list **token_list);

#endif