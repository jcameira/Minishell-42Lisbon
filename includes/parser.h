/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:54:08 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/29 01:41:45 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <termios.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <libft.h>
# include <sys/wait.h>
# include <get_next_line_bonus.h>

# define HERE_DOC_INDICATOR "> "
# define INPUT 0
# define READ 0
# define WRITE 1

# define SUCCESS 1
# define FAILURE 0

# define NO_SPACE "No more space left in device\n"
# define NO_HERE_DOC_LINE_START "\nminishell: warning: here-document at line "
# define NO_HERE_DOC_LINE_MIDDLE " delimited by end-of-file (wanted `"
# define NO_HERE_DOC_LINE_END "')\n"
# define OPEN_PIPE_ERROR "Error opening pipe\n"
# define FORK_ERROR "Error creating fork\n"

# define PRINT_NO_TYPE	"NO_TYPE"
# define PRINT_AND	"AND"
# define PRINT_OR	"OR"
# define PRINT_PIPE	"PIPE"
# define PRINT_LESSER	"LESSER"
# define PRINT_D_LESSER	"D_LESSER"
# define PRINT_GREATER	"GREATER"
# define PRINT_D_GREATER	"D_GREATER"
# define PRINT_SUBSHELL	"SUBSHELL"
# define PRINT_L_PARENTESIS	"L_PARENTESIS"
# define PRINT_R_PARENTESIS	"R_PARENTESIS"
# define PRINT_WORD	"WORD"

# define PRINT_NO_NODE "NO_NODE"
# define PRINT_BAD_TYPE "BAD_TYPE"
# define PRINT_AND_OR_SEQUENCE "AND_OR_SEQUENCE"
# define PRINT_PIPE_SEQUENCE "PIPE_SEQUENCE"
# define PRINT_SUBSHELL "SUBSHELL"
# define PRINT_REDIRECTION "REDIRECTION"
# define PRINT_SIMPLE_COMMAND "SIMPLE_COMMAND"

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

typedef enum s_ast_node_type
{
	NO_NODE = -2,
	BAD_TYPE,
	AND_OR_SEQUENCE,
	PIPE_SEQUENCE,
	SUBSHELL,
	REDIRECTION,
	SIMPLE_COMMAND
}				t_ast_token_type;

typedef enum s_token_list_type
{
	NO_TYPE = 0,
	AND,
	OR,
	PIPE,
	LESSER,
	D_LESSER,
	GREATER,
	D_GREATER,
	L_PARENTESIS,
	R_PARENTESIS,
	WORD,
	BAD_TOKEN
}				t_token_list_type;

typedef enum s_symbol
{
	NO_SYMBOL = -1,
	S_PIPE,
	S_AND,
	S_OR
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

typedef struct s_token_list
{
	t_token_list_type	token_type;
	char				*content;
	struct s_token_list	*next;
}				t_token_list;

typedef struct s_ast
{
	struct s_ast		*original_root;
	t_ast_token_type	type;
	char				*content;
	int					subshell_level;
	int					visited;
	struct s_ast		*subshell_ast;
	struct s_ast		*left;
	struct s_ast		*right;
}				t_ast;

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

t_ast_token_type	set_ast_node_type(t_token_list *token_node);
char				*get_node_content(t_token_list *token_node);
char				*get_subshell_content(t_token_list *token_node);
char				*get_simple_command_content(t_token_list *token_node);
int					check_for_subshell(t_token_list *token_list);
int					check_for_node(t_token_list *token_list,
						t_ast_token_type type);
void				trim_parentesis_nodes(t_token_list **token_list);
t_ast				*new_ast_node(t_token_list *token_node, int subshell_lvl);
void				free_token_list_node(t_token_list **node);
void				print_list(t_token_list *token_list);
void				free_token_list(t_token_list *list);
t_ast				*add_ast_node(t_token_list **token_list, int subshell_lvl);
t_ast				*new_regular_node(t_token_list **token_list,
						t_ast_token_type type, int subshell_lvl);
t_ast				*new_subshell_node(t_token_list **token_list,
						int subshell_lvl);
void				skip_subshell(t_token_list **token_list);
void				free_ast(t_ast *root);
t_token_list		*search_list_for_token(t_token_list *token_list,
						t_ast_token_type type);
void				separate_list(t_token_list **token_list,
						t_token_list **left_list, t_token_list **right_list,
						t_token_list_type type);
int					array_size(char **array);
char				**arrcpy(char **dest, char **src);
void				free_arr(char **array);
t_table_node_type	set_table_node_type(t_ast *root);
t_redir_type		set_redir_type(char *content);
char				*set_redir_str(char *dest, t_ast **root);
t_command_table		*last_table_node(t_command_table *command_table);
t_redir_list		*last_redir_node(t_redir_list *redir_list);
void				add_new_table_node(t_command_table **command_table,
						t_command_table *new);
t_command_table		*new_command_table_node(t_ast *root);
t_redir_list		*new_command_table_redir(t_minishell *msh, t_ast **root,
						t_command_table *command_table);
t_simplecmd			*new_command_table_simple_command(t_ast *root);
void				add_simple_command_argument(t_ast *root,
						t_command_table **node);
void				add_more_content_to_table_node(t_minishell *msh,
						t_ast **root, t_command_table **command_table);
void				create_command_table(t_minishell *msh, t_ast *root,
						t_command_table **command_table);
void				free_command_table(t_command_table *command_table,
						int close_all_fds);
int					expander(t_minishell *msh, t_command_table *command_table);
void				free_token_list(t_token_list *list);
t_redir_list		*set_redir_values(t_minishell *msh, t_ast **root,
						t_command_table *command_table, t_redir_list *redirs);
void				set_here_doc_expansion(t_redir_list **redir);
int					str_len_no_quotes(char *content);
char				*set_no_quotes_content(char	*content, int real_len);
char				*remove_quotes(char *content);
char				*get_env_value(t_minishell *msh, char *env_name);
char				*get_env_name(char *content, int *i);
int					get_env_variable_len(t_minishell *msh, t_command_table *table, char *content,
						int *i);
char				*add_expanded_var(char *env_value, char *expanded_content,
						int *j);
int					isenvchar(int c);
int					fork_here_doc(t_minishell *msh, t_ast *root,
						t_command_table *command_table, t_redir_list **redirs);
char				*expansion_inside_here_doc(t_minishell *msh, t_command_table *table, char *content,
						int flag);
void				here_doc_signals_init(void);
void				exit_shell(t_minishell *msh, int exit_code);
void				free_redir_list(t_redir_list *redirs, int close_all_fds);
void				visit_node(t_minishell *msh, t_ast **root,
						t_command_table **command_table);
t_final_cmd_table	*create_final_cmd_table(t_minishell *msh, t_command_table *command_table);
int					executor(t_minishell *msh, t_final_cmd_table *final_cmd_table);

#endif