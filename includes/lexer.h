/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:02:12 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/03 16:23:48 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>
# include <libft.h>

# define UNCLOSED_S_QUOTES "minishell: syntax error unclosed single quotes\n"
# define UNCLOSED_D_QUOTES "minishell: syntax error unclosed double quotes\n"
# define UNCLOSED_PARENTESIS "minishell: syntax error unclosed parentesis\n"

typedef enum s_tokens
{
	AND = 1,
	OR,
	PIPE,
	LESSER,
	D_LESSER,
	GREATER,
	D_GREATER
}				t_tokens;

typedef struct s_ast
{
	char			*type;
	char			*content;
	struct s_ast	*left;
	struct s_ast	*right;
}				t_ast;

void	lexer(char *line);
int		check_odd_number_tokens(char *line, bool *subshell);
void	open_subshell(char *line);

#endif