/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/25 15:05:35 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include <libft.h>

# define NO_SPACE "No more space left in device\n"

# define DEFAULT_CMD_PATH "/usr/bin/"

# define SUCCESS 1
# define FAILURE 0

# define OPEN_IN_ERROR "Error opening infile\n"
# define OPEN_OUT_ERROR "Error opening outfile\n"
# define OPEN_PIPE_ERROR "Error opening pipe\n"

# define EXECUTION_ERROR_MSG_PREFIX "minishell: "
# define COMMAND_ERROR_MSG ": command not found\n"
# define AMBIGUOUS_REDIRECT "minishell: %s: ambiguous redirect\n"

# define COMMAND_NOT_FOUND_CODE 127

# define READ 0
# define WRITE 1

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef enum s_redir_type
{
	NO_REDIR = -1,
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
	char				**export_list;
	char				*private_path;
	char				*prompt;
	int					exit_code;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

typedef struct s_final_cmd_table
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
	struct s_final_cmd_table		*next;
}				t_final_cmd_table;
typedef struct s_execution_info
{
	t_final_cmd_table		*tmp_table;
	pid_t					*pid;
	int						pipeline_start;
	int						pipeline_size;
	int						in_pipe[2];
	int						out_pipe[2];
}				t_execution_info;

void				free_f_command_table(t_final_cmd_table *command_table);
void				free_arr(char **array);
void				exit_shell(t_minishell *msh, int exit_code);
void				close_pipes(t_execution_info *info);
void				free_f_command_table_node(t_final_cmd_table **cmd_table);
int					execute_in_fork(t_minishell *msh, t_execution_info *info,
						int *i);
int					set_in(t_final_cmd_table *final_cmd_table);
int					set_out(t_final_cmd_table *final_cmd_table);
void				reset_std_fds(t_minishell *msh);
int					get_pipeline_size(t_final_cmd_table *final_cmd_table);
void				check_if_pipefd_needed(t_execution_info **info);
t_execution_info	*exec_info_init(t_final_cmd_table *final_cmd_table);
int					init_pipeline(t_execution_info **info, int *i);
void				child_signals_init(void);
void				ignore_signals_init(void);
void				set_dups(t_execution_info *info, int fd_in, int fd_out, int *i);

#endif