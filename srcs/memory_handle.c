/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 01:14:41 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/17 17:20:48 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_arr(char **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	exit_shell(t_minishell *msh, int exit_code)
{
	free_arr(msh->envp);
	free_arr(msh->export_list);
	close(msh->original_stdin);
	close(msh->original_stdout);
	close(msh->original_stderr);
	free(msh);
	exit(exit_code);
}
