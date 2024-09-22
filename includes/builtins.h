/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/22 02:21:41 by jcameira         ###   ########.fr       */
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

# define NO_SPACE "No more space left in device\n"

# define CD_NO_FILE_DIR "minishell: cd: %s: No such file or directory"
# define CD_TOO_MANY_ARGS "minishell: cd: too many arguments\n"
# define CD_NO_HOME "minishell: cd: HOME not set\n"
# define CD_NO_OLDPWD "minishell: cd: OLDPWD not set\n"

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
	char				*prompt;
	int					exit_code;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

void	mini_cd(t_minishell *msh, t_simplecmd *cmd);
void	mini_echo(t_minishell *msh, t_simplecmd *cmd);
void	mini_env(t_minishell *msh, t_simplecmd *cmd);
void	mini_export(t_minishell *msh, t_simplecmd *cmd);
void	mini_pwd(t_minishell *msh, t_simplecmd *cmd);
void	mini_unset(t_minishell *msh, t_simplecmd *cmd);
void	exit_shell(t_minishell *msh, int exit_code);
void	free_arr(char **array);
int		array_size(char **array);
char	*find_path(t_minishell *msh, t_simplecmd *cmd, char *macro);
char	*parent_dir(char *path);
char	**arrcpy(char **dest, char **src);
char	**split_by_char(char *str, char c);

#endif