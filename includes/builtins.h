/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/07 18:23:47 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include <libft.h>

// extern int g_signal;

# define NO_SPACE "No more space left in device\n"

# define ENV_PREFIX "env: ‘"
# define ENV_FILE_DIR "’: No such file or directory\n"

# define CD_PREFIX "minishell: cd: "
# define CD_NO_FILE_DIR ": No such file or directory\n"
# define CD_TOO_MANY_ARGS "minishell: cd: too many arguments\n"
# define CD_NO_HOME "minishell: cd: HOME not set\n"
# define CD_NO_OLDPWD "minishell: cd: OLDPWD not set\n"
# define CD_CANNOT_ACCESS_PARENT_DIR "chdir: error retrieving current \
directory: getcwd: cannot access parent directories: No such file or directory\n"

# define EXPORT_PREFIX "minishell: export: `"
# define EXPORT_NOT_IDENTIFIER "': not a valid identifier\n"

# define EXIT_PREFIX "minishell: exit: "
# define EXIT_NON_NUMERIC_ARGUMENT ": numeric argument required\n"
# define EXIT_TOO_MANY_ARGUMENTS "minishell: exit: too many arguments\n"

# define PWD_CANNOT_ACCESS_PARENT_DIR "pwd: error retrieving current directory:\
 getcwd: cannot access parent directories: No such file or directory\n"

# define PWD "PWD="
# define OLDPWD "OLDPWD="
# define HOME "HOME="
# define PATH "PATH="

# define SUCCESS 1
# define FAILURE 0

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

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

int		mini_cd(t_minishell *msh, t_simplecmd *cmd);
int		mini_echo(t_minishell *msh, t_simplecmd *cmd);
int		mini_env(t_minishell *msh, t_simplecmd *cmd);
int		mini_export(t_minishell *msh, t_simplecmd *cmd);
int		mini_pwd(t_minishell *msh, t_simplecmd *cmd);
int		mini_unset(t_minishell *msh, t_simplecmd *cmd);
int		mini_exit(t_minishell *msh, t_simplecmd *cmd);
void	exit_shell(t_minishell *msh, int exit_code);
void	free_arr(char **array);
int		array_size(char **array);
char	*find_path(t_minishell *msh, char *macro);
char	*parent_dir(char *path);
char	**arrcpy(char **dest, char **src);
char	**split_by_char(char *str, char c);
int		isenvchar(int c);
int		check_non_identifier_char(char **str, char *full_str);
void	bubble_sort(char **matrix);

#endif