/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:40:24 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/04 18:59:47 by jcameira         ###   ########.fr       */
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

# define S 0
# define D 1

typedef enum s_quote_flag
{
	QUOTES = 0,
	PARAMETERS,
	WILDCARDS
}				t_quote_flag;

typedef enum s_redir_type
{
	INFILE = 0,
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
	char				*here_doc_buffer;
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
	char				*prompt;
	int					original_stdin;
	int					original_stdout;
	int					original_stderr;
}				t_minishell;

void	free_command_table(t_command_table *command_table);
void	print_cmd_table(t_command_table *command_table);
void	free_command_table(t_command_table *command_table);
void	free_arr(char **array);
int		quote_removal_str_len(char *content);
char	*remove_quotes_expansion(char *content, int len);
int		parameter_expansion_str_len(t_minishell *msh, char *content);
char	*expand_parameter(t_minishell *msh, char *content, int len);
int		needs_wildcard_expansion(char *content);
int		wildcards_str_len(char *content);
char	*expand_wildcards(char *content, int len, int needs_expansion);
int		str_len_no_quotes(char *content);
char	*set_no_quotes_content(char	*content, int real_len);
int		isenvchar(int c);
char	*get_env_value(t_minishell *msh, char *env_name);
char	*get_env_name(char *content, int *i);
int		get_env_variable_len(t_minishell *msh, char *content, int *i);
char	*add_expanded_parameter(t_minishell *msh, char *content,
			char *new_content, int *indexes);
int		expansion_inside_quotes(char *content, int i, char c,
			t_quote_flag flag);
int		len_inside_quotes(char *content, int *i, char c);
void	add_content_inside_quotes(char *content, char *new_content, int *i,
			int *j);
int		match_wildcard_pattern(char *pattern, char *file);
DIR		*get_directory_info(DIR *directory, char **new_content,
			struct dirent **file, int flag);
char	*append_more_wildcard_content(char *new_content, struct dirent *file);
#endif