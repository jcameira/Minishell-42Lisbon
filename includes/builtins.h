/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/07/10 18:57:28 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <minishell.h>
# include <libft.h>

# define PWD "PWD="
# define OLDPWD "OLDPWD="
# define HOME "HOME="

typedef struct s_simplecmd
{
	int		arg_nbr;
	char	**arg_arr;
}				t_simplecmd;

void	mini_cd(t_minishell *msh, t_simplecmd *cmd);
void	mini_echo(t_minishell *msh, t_simplecmd *cmd);
void	mini_env(t_minishell *msh, t_simplecmd *cmd);
void	mini_export(t_minishell *msh, t_simplecmd *cmd);
void	mini_pwd(t_minishell *msh, t_simplecmd *cmd);
void	mini_unset(t_minishell *msh, t_simplecmd *cmd);
char	*find_path(t_minishell *msh, t_simplecmd *cmd, char *macro);
char	*find_slash(char *path);
char	**arrdup(char **array);
#endif