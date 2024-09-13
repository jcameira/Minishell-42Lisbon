/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:02:12 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/13 15:39:59 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <libft.h>

# define WHITESPACE "\t\n\v\f\r "

# define UNCLOSED_S_QUOTES "minishell: syntax error unclosed single quotes\n"
# define UNCLOSED_D_QUOTES "minishell: syntax error unclosed double quotes\n"
# define UNCLOSED_PARENTESIS "minishell: syntax error unclosed parentesis\n"

# define NO_SPACE "No more space left in device\n"

# define PRINT_NO_TYPE	"NO_TYPE"
# define PRINT_AND	"AND"
# define PRINT_OR	"OR"
# define PRINT_PIPE	"PIPE"
# define PRINT_LESSER	"LESSER"
# define PRINT_D_LESSER	"D_LESSER"
# define PRINT_GREATER	"GREATER"
# define PRINT_D_GREATER	"D_GREATER"
# define PRINT_L_PARENTESIS	"L_PARENTESIS"
# define PRINT_R_PARENTESIS	"R_PARENTESIS"
# define PRINT_WORD	"WORD"
# define PRINT_BAD_TOKEN	"BAD_TOKEN"
# define PRINT_END	"END"

# define AND_PARSE_ERROR "minishell: parse error near '&&'\n"
# define OR_PARSE_ERROR "minishell: parse error near '||'\n"
# define PIPE_PARSE_ERROR "minishell: parse error near '|'\n"
# define OPEN_PARENTESIS_PARSE_ERROR "minishell: parse error near '('\n"
# define CLOSE_PARENTESIS_PARSE_ERROR "minishell: parse error near ')'\n"
# define INFILE_PARSE_ERROR "minishell: parse error near '<'\n"
# define OUTFILE_PARSE_ERROR "minishell: parse error near '>'\n"
# define APPEND_PARSE_ERROR "minishell: parse error near '>>'\n"
# define HERE_DOC_PARSE_ERROR "minishell: parse error near '<<'\n"

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
	BAD_TOKEN,
	END
}				t_token_list_type;

typedef struct s_token_list
{
	t_token_list_type	token_type;
	char				*content;
	struct s_token_list	*next;
}				t_token_list;

typedef struct s_minishell
{
	char				**envp;
	char				*prompt;
	int					exit_code;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

void				free_token_list(t_token_list *list);
void				free_arr(char **array);
int					lexer(t_minishell *msh, char *line);
int					check_odd_number_tokens(char *line);
t_token_list		*set_token_list_types(t_token_list *token_list);
t_token_list_type	set_token_type(char	*c);
t_token_list		*new_token(int type, char *data);
void				add_new_token(t_token_list **token_list, t_token_list *new);
void				add_token_middle_list(t_token_list **token_list,
						t_token_list *new);
t_token_list		*last_token(t_token_list *token_list);
void				skip_until_char(char *line, int *i, char c);
t_token_list		*get_initial_list(char *line);
t_token_list		*refine_list(t_token_list *initial_list);
int					is_operator_token(char	*c);
t_token_list		*split_operator_tokens(t_token_list *node);
int					parser(t_minishell *msh, t_token_list *token_list);
int					check_syntax_errors(t_token_list *token_list);
int					check_and_or_syntax_errors(t_token_list *token_list,
						t_token_list *previous);
int					check_pipe_syntax_errors(t_token_list *token_list,
						t_token_list *previous);
int					check_subshell_syntax_errors(t_token_list *token_list);
int					check_redirection_syntax_errors(t_token_list *token_list);

#endif