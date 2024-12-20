/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:47:42 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 17:36:49 by jcameira         ###   ########.fr       */
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

extern int	g_signal;

# define MSH_PROMPT "Minishell >$"
# define SHLVL "SHLVL="
# define SET_SHLVL "SHLVL=1"
# define PWD "PWD="
# define PRIVATE_PATH "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin\
:/usr/bin:/sbin:/bin"

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

# define SUCCESS 1
# define FAILURE 0

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

# define WRONG_ARG_N "Minishell needs to be executed with no additional \
arguments.\n"
# define NO_SPACE "No more space left in device\n"

typedef struct s_minishell
{
	char				**envp;
	char				**export_list;
	char				*private_path;
	char				*prompt;
	int					exit_code;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

char	**arrdup(char **array);
void	minishell_init(t_minishell *msh, char **envp);
int		array_size(char **array);
char	**increment_shlvl(char **array);
char	**set_shlvl(char **array);
char	**set_pwd(char **array);
int		full_white_space(char *line);
void	free_arr(char **array);
void	interactive_signals_init(void);
void	interactive_handler(int sig);
void	child_handler(int sig);
void	here_handler(int sig);
void	exit_shell(t_minishell *msh, int exit_code);
void	bubble_sort(char **matrix);
int		lexer(t_minishell *msh, char *line);

#endif