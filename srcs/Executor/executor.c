/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:22:22 by jcameira          #+#    #+#             */
/*   Updated: 2024/07/10 00:26:25 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->just execute the damn thing, need to think of a much more computation light
//way to do it then what was done in pipex, we can't just open a million pipes
//all at the same time for example

#include <executor.h>

void	executor(t_minishell *msh, t_final_command_table *final_command_table)
{
	(void)msh;
	free_f_command_table(final_command_table);
}
