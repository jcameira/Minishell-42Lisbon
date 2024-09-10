/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 01:14:41 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/10 17:44:37 by jcameira         ###   ########.fr       */
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

void	exit_shell(t_minishell *msh)
{
	free_arr(msh->envp);
	close(msh->original_stdin);
	close(msh->original_stdout);
	close(msh->original_stderr);
	free(msh);
	printf("exit\n");
	exit(1);
}
