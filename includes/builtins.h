/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:04:23 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/06 21:02:22 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "minishell.h"

void	mini_cd(t_minishell *msh, t_simplecmd *cmd);
void	mini_echo(t_minishell *msh, t_simplecmd *cmd);
void	mini_env(t_minishell *msh, t_simplecmd *cmd);
void	mini_export(t_minishell *msh, t_simplecmd *cmd);
void	mini_pwd(t_minishell *msh, t_simplecmd *cmd);
void	mini_unset(t_minishell *msh, t_simplecmd *cmd);

#endif