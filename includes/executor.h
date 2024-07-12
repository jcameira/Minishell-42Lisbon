/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/07/11 20:04:21 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <minishell.h>

typedef enum s_redir_type
{
	INFILE = 0,
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
	//t_final_redirs					redirs;
	t_redir_type					in_type;
	char							*infile;
	t_redir_type					out_type;
	char							*outfile;
	int								ambiguous_redirect;
	t_next_symbol					next_symbol;
	struct s_final_command_table	*next;
}				t_final_command_table;

void	free_f_command_table(t_final_command_table *command_table);

#endif