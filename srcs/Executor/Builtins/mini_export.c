/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:59 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/06 18:02:45 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	**add_env(char **array, char *new_env)
{
	char	**new_arr;
	int		size;
	int		i;

	if (!(*array))
		return (NULL);
	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 2));
	i = -1;
	while (array[++i])
		new_arr[i] = ft_strdup(array[i]);
	new_arr[i] = ft_strdup(new_env);
	new_arr[i + 1] = NULL;
	free_arr(array);
	return (new_arr);
}

void	mini_export(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;
	
	i = -1;
	
	while (msh->envp[++i])
	{
		if (!ft_strncmp(cmd->arg_arr[1], msh->envp[i], ft_strlen(cmd->arg_arr[1])))
		{
			free(msh->envp[i]);
			msh->envp[i] = ft_strdup(cmd->arg_arr[1]);
			return ;
		}
	}
	msh->envp[i] = add_env(msh->envp[i], cmd->arg_arr[1]);
}