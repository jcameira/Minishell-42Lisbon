/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:40:24 by jcameira          #+#    #+#             */
/*   Updated: 2024/06/20 20:48:21 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <libft.h>

# define NO_SPACE "No more space left in device\n"

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
	int					expand_here_doc;
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

typedef struct s_minishell
{
	char				**envp;
	char				*prompt;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

void	free_command_table(t_command_table *command_table);



void	print_cmd_table(t_command_table *command_table);
void	free_command_table(t_command_table *command_table);
#endif