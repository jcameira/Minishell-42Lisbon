/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/07/19 18:14:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "minishell.h"

# define PWD "PWD="
# define OLDPWD "OLDPWD="
# define HOME "HOME="
# define PATH "PATH="

void	mini_cd(t_minishell *msh, t_simplecmd *cmd);
void	mini_echo(t_minishell *msh, t_simplecmd *cmd);
void	mini_env(t_minishell *msh, t_simplecmd *cmd);
void	mini_export(t_minishell *msh, t_simplecmd *cmd);
void	mini_pwd(t_minishell *msh, t_simplecmd *cmd);
void	mini_unset(t_minishell *msh, t_simplecmd *cmd);

#endif