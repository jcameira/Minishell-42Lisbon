/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:47:42 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/02 03:34:26 by jcameira         ###   ########.fr       */
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
# include <termios.h>
# include <libft.h>

# define MSH_PROMPT "Minishell >$"
# define SHLVL "SHLVL="
# define SET_SHLVL "SHLVL=1"
# define PWD "PWD="

# define WRONG_ARG_N "Minishell needs to be executed with no additional \
arguments.\n"

typedef struct s_ast
{
	char			*type;
	char			*content;
	struct s_ast	*left;
	struct s_ast	*right;
}				t_ast;
typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

typedef struct s_command
{
	t_simplecmd	**simplecmd_arr;
	int			simplecmd_nbr;
	char		*infile;
	int			fd_in;
	char		*outfile;
	int			fd_out;
	bool		here_doc;
	bool		builtin;
}				t_command;

typedef struct s_minishell
{
	char				**envp;
	char				*prompt;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
	// struct sigaction	interactive_sa;
}				t_minishell;

char	**arrdup(char **array);
void	minishell_init(t_minishell *msh, char **envp);
int		array_size(char **array);
char	**increment_shlvl(char **array);
char	**set_shlvl(char **array);
char	**set_pwd(char **array);
int		is_white_space(char *line);
void	free_arr(char **array);
void	signals_init(void);
void	interactive_handler(int sig);
void	exit_shell(t_minishell *msh);

#endif