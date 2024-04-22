/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:47:42 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/22 18:49:38 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>

# define WRONG_ARG_N "Minishell needs to be executed with no additional arguments.\n"

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

typedef struct s_command
{
	t_simplecmd	**simplecmd_arr;
	int 		simplecmd_nbr;
	char		*infile;
	int			fd_in;
	char		*outfile;
	int			fd_out;
	bool		here_doc;
	bool		builtin;
}				t_command;

typedef struct s_minishell
{
	char		**envp;
	int			stdin;
	int			stdout;
}				t_minishell;

#endif