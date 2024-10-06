/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:48:08 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/05 17:40:41 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	**rm_env(char **array, char *old_env)
{
	char	**new_arr;
	int		size;
	int		i;
	int		j;

	if (!(*array))
		return (NULL);
	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size));
	i = -1;
	j = -1;
	while (array[++i])
	{
		if (!ft_strncmp(array[i], old_env, ft_strlen(old_env))
			&& array[i][ft_strlen(old_env)] == '=')
			continue ;
		new_arr[++j] = ft_strdup(array[i]);
	}
	new_arr[++j] = NULL;
	free_arr(array);
	return (new_arr);
}

void	unset_aux(t_minishell *msh, t_simplecmd *cmd, int j)
{
	int	i;

	i = -1;
	while (msh->envp[++i])
	{
		if (!ft_strncmp(cmd->arg_arr[j], "PATH",
				ft_strlen("PATH")) && msh->private_path)
		{
			free(msh->private_path);
			msh->private_path = NULL;
		}
		if (!ft_strncmp(cmd->arg_arr[j], msh->envp[i],
				ft_strlen(cmd->arg_arr[j]))
			&& msh->envp[i][ft_strlen(cmd->arg_arr[j])] == '=')
		{
			msh->envp = rm_env(msh->envp, cmd->arg_arr[j]);
			msh->export_list = rm_env(msh->export_list, cmd->arg_arr[j]);
			--i;
		}
	}
}

int	mini_unset(t_minishell *msh, t_simplecmd *cmd)
{
	int		i;

	i = 0;
	if (cmd->arg_nbr == 1)
		return (EXIT_SUCCESS);
	while (cmd->arg_arr[++i])
		unset_aux(msh, cmd, i);
	return (EXIT_SUCCESS);
}
