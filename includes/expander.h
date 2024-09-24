/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:40:24 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 20:00:12 by jcameira         ###   ########.fr       */
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
# include <dirent.h>
# include <libft.h>

# define NO_SPACE "No more space left in device\n"

# define EXPAND_SUBSHELL "MINISHELL_SUBSHELL"

# define S 0
# define D 1

typedef enum s_quote_flag
{
	QUOTES = 0,
	PARAMETERS,
	WILDCARDS,
	AMBIGUOUS
}				t_quote_flag;

typedef enum s_redir_type
{
	NO_REDIR = -1,
	INFILE,
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

typedef struct s_redir_list
{
	t_redir_type		type;
	char				*file;
	char				*here_doc_limiter;
	int					here_doc_fd;
	int					expand_here_doc;
	int					ambiguous_redirect;
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

void					free_command_table(t_command_table *command_table,
							int close_all_fds);
void					print_cmd_table(t_command_table *command_table);
void					free_arr(char **array);
int						quote_removal_str_len(char *content);
char					*remove_quotes_expansion(char *content, int len);
int						parameter_expansion_str_len(t_minishell *msh,
							t_command_table *table, char *content);
char					*expand_parameter(t_minishell *msh,
							t_command_table *table, char *content, int len);
int						needs_wildcard_expansion(char *content);
int						wildcards_str_len(char *content);
char					*expand_wildcards(char *content, int len,
							int needs_expansion);
int						str_len_no_quotes(char *content);
char					*set_no_quotes_content(char	*content, int real_len);
int						isenvchar(int c);
char					*get_env_value(t_minishell *msh, char *env_name);
char					*get_env_name(char *content, int *i);
int						get_env_variable_len(t_minishell *msh,
							t_command_table *table, char *content, int *i);
char					*add_expanded_parameter(t_minishell *msh,
							t_command_table *table, char **contents,
							int (*indexes)[2]);
int						expansion_inside_quotes(char *content, int i, char c,
							t_quote_flag flag);
int						len_inside_quotes(char *content, int *i, char c);
void					add_content_inside_quotes(char *content,
							char *new_content, int *i, int *j);
int						match_wildcard_pattern(char *pattern, char *file);
DIR						*get_directory_info(DIR *directory, char **new_content,
							struct dirent **file, int flag);
char					*append_more_wildcard_content(char *new_content,
							struct dirent *file);
t_final_cmd_table		*set_final_redirs(t_final_cmd_table	*new_table_node,
							t_redir_list *redirs);
void					free_redir_list(t_redir_list *redirs,
							int close_all_fds);
void					free_f_command_table(t_final_cmd_table *cmd_table);
t_simplecmd				*simplecmdcpy(t_simplecmd *simplecmd);
t_symbol				check_next_symbol(t_command_table *next_node);
char					**arrdup(char **array);
int						executor(t_minishell *msh,
							t_final_cmd_table *final_cmd_table);
t_final_cmd_table		*create_final_cmd_table(t_command_table *command_table);
void					skip_until_char(char *line, int *i, char c);
int						(*builtin_arr(char *command))(t_minishell *msh,
							t_simplecmd *cmd);
void					free_symbol_node(t_command_table **command_table);
t_final_cmd_table		*set_redir_info(t_final_cmd_table *new_table_node,
							t_redir_list *redirs);
t_final_cmd_table		*set_redir_info_infile(t_final_cmd_table *node,
							t_redir_list *redirs);
int						array_size(char **array);
int						has_space(char *content);
int						space_not_in_quotes(char *str);
int						set_ambiguous_redirect(t_minishell *msh,
							t_command_table *table, char *content);

#endif