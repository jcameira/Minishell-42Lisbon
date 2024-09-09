/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/09 21:07:17 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <libft.h>

# define DEFAULT_CMD_PATH "/usr/bin/"

# define OPEN_IN_ERROR "Error opening infile\n"
# define OPEN_OUT_ERROR "Error opening outfile\n"
# define OPEN_PIPE_ERROR "Error opening pipe\n"

# define READ 0
# define WRITE 1

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef enum s_redir_type
{
	NO_TYPE = -1,
	INFILE,
	OUTFILE,
	APPEND,
	HERE_DOC
}				t_redir_type;

typedef enum s_symbol
{
	NO_SYMBOL = -1,
	PIPE,
	AND,
	OR
}				t_symbol;

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

typedef struct s_minishell
{
	char				**envp;
	char				*prompt;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

typedef struct s_final_command_table
{
	int								subshell_level;
	t_simplecmd						*simplecmd;
	int								(*builtin)(t_minishell *, t_simplecmd *);
	t_redir_type					in_type;
	char							*infile;
	int								infile_fd;
	int								here_doc_fd;
	t_redir_type					out_type;
	char							*outfile;
	int								outfile_fd;
	int								ambiguous_redirect;
	t_symbol						previous_symbol;
	t_symbol						next_symbol;
	struct s_final_command_table	*next;
}				t_final_command_table;

void	free_f_command_table(t_final_command_table *command_table);

#endif