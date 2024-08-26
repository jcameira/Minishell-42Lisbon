/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/08/26 20:49:36 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <minishell.h>

typedef enum s_redir_type
{
	NO_TYPE = -1,
	INFILE,
	OUTFILE,
	APPEND,
	HERE_DOC
}				t_redir_type;

typedef enum s_next_symbol
{
	NO_SYMBOL = -1,
	PIPE,
	AND,
	OR
}				t_next_symbol;

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

typedef struct s_final_command_table
{
	int								subshell_level;
	t_simplecmd						*simplecmd;
	t_redir_type					in_type;
	char							*infile;
	int								infile_fd;
	int								here_doc_fd;
	t_redir_type					out_type;
	char							*outfile;
	int								outfile_fd;
	int								ambiguous_redirect;
	t_next_symbol					next_symbol;
	struct s_final_command_table	*next;
}				t_final_command_table;

void	free_f_command_table(t_final_command_table *command_table);

#endif