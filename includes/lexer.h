/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:02:12 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/13 11:55:05 by jcameira         ###   ########.fr       */
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

void			free_token_list(t_token_list *list);
void			free_arr(char **array);
void			lexer(char *line);
int				check_odd_number_tokens(char *line);
t_token_list	*set_token_types(t_token_list *token_list);
int				set_token_type(char	*c);
t_token_list	*new_token(int type, char *data);
void			add_new_token(t_token_list **token_list, t_token_list *new);
void			add_token_middle_list(t_token_list **token_list,
					t_token_list *new);
t_token_list	*last_token(t_token_list *token_list);
void			skip_until_char(char *line, int *i, char c);
t_token_list	*get_initial_list(char *line);
t_token_list	*refine_list(t_token_list *initial_list);
int				is_operator_token(char	*c);
t_token_list	*split_operator_tokens(t_token_list *node);

#endif