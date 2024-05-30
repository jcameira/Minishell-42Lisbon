/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:54:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/30 22:55:45 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
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

# define PRINT_NO_NODE "NO_NODE"
# define PRINT_BAD_TYPE "BAD_TYPE"
# define PRINT_AND_OR_SEQUENCE "AND_OR_SEQUENCE"
# define PRINT_PIPE_SEQUENCE "PIPE_SEQUENCE"
# define PRINT_SUBSHELL "SUBSHELL"
# define PRINT_REDIRECTION "REDIRECTION"
# define PRINT_SIMPLE_COMMAND "SIMPLE_COMMAND"

typedef enum s_redir_type
{
	INFILE = 0,
	OUTFILE,
	APPEND,
	HERE_DOC
}				t_redir_type;

typedef enum s_table_node_type
{
	TABLE_NO_TYPE = -1,
	TABLE_AND,
	TABLE_OR,
	TABLE_PIPE,
	TABLE_SUBSHELL
}				t_table_node_type;

typedef enum s_ast_node_type
{
	NO_NODE = -2,
	BAD_TYPE,
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

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

typedef struct s_redir_list
{
	t_redir_type		type;
	char				*file;
	char				*here_doc_limiter;
	struct s_redir_list	*next;
}				t_redir_list;

typedef struct s_command_table
{
	t_table_node_type		type;
	int						subshell_level;
	t_simplecmd				*simplecmd;
	t_redir_list			*redirs;
	struct s_command_table	*next;
}				t_command_table;
typedef struct s_token_list
{
	t_token_list_type	token_type;
	char				*content;
	struct s_token_list	*next;
}				t_token_list;

typedef struct s_ast
{
	t_ast_token_type	type;
	char				*content;
	int					subshell_level;
	struct s_ast		*subshell_ast;
	struct s_ast		*left;
	struct s_ast		*right;
}				t_ast;

t_ast_token_type	set_ast_node_type(t_token_list *token_node);
char				*get_node_content(t_token_list *token_node);
char				*get_subshell_content(t_token_list *token_node);
char				*get_simple_command_content(t_token_list *token_node);
int					check_for_subshell(t_token_list *token_list);
int					check_for_node(t_token_list *token_list,
						t_ast_token_type type);
void				trim_parentesis_nodes(t_token_list **token_list);
t_ast				*new_ast_node(t_token_list *token_node, int subshell_lvl);
void				free_token_list_node(t_token_list **node);
void				print_list(t_token_list *token_list);
void				free_token_list(t_token_list *list);
t_ast				*add_ast_node(t_token_list **token_list, int subshell_lvl);
t_ast				*new_regular_node(t_token_list **token_list,
						t_ast_token_type type, int subshell_lvl);
t_ast				*new_subshell_node(t_token_list **token_list, int subshell_lvl);
void				skip_subshell(t_token_list **token_list);
void				free_ast(t_ast *root);
t_token_list		*search_list_for_token(t_token_list *token_list,
						t_ast_token_type type);
void				separate_list(t_token_list **token_list,
						t_token_list **left_list, t_token_list **right_list,
						t_token_list_type type);
void				create_command_table(t_ast *root, t_command_table **command_table);
#endif