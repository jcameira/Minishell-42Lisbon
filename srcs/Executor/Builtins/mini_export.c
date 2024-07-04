/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:59 by mpais-go          #+#    #+#             */
/*   Updated: 2024/07/04 01:51:33 by jcameira         ###   ########.fr       */
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

void	print_declare(char **env)
{
	char	**copy_env;
	char	**tmp;
	int		i;

	i = -1;
	copy_env = arrdup(env);
	bubble_sort(copy_env);
	while (copy_env[++i])
	{
		if (!ft_strncmp(copy_env[i], "_=", 2))
			continue ;
		tmp = ft_split(copy_env[i], '=');
		printf("declare -x %s", tmp[0]);
		if (!tmp[1])
			printf("\n");
		else
			printf("=\"%s\"\n", tmp[1]);
		free_arr(tmp);
	}
}

void	aux_export(char **msh_envp, char *cmd_argarr, char **tmp_cmd)
{
	char	**tmp_env;
	int		i;

	i = -1;
	while (msh_envp[++i])
	{
		tmp_env = ft_split(msh_envp[i], '=');
		if (!tmp_cmd[1]
			&& !ft_strncmp(tmp_cmd[0], tmp_env[0], ft_strlen(tmp_cmd[0])))
			break ;
		if (!msh_envp[i + 1] || (tmp_cmd[1] && !ft_strncmp(tmp_cmd[0],
					tmp_env[0], ft_strlen(tmp_cmd[0]))))
		{
			free(msh_envp[i]);
			msh_envp[i] = ft_strdup(cmd_argarr);
			free_arr(tmp_cmd);
			free_arr(tmp_env);
		}
	}
}

void	mini_export(t_minishell *msh, t_simplecmd *cmd)
{
	char	**tmp_cmd;
	int		i;

	if (cmd->arg_nbr == 1)
		print_declare(msh->envp);
	i = 0;
	while (cmd->arg_arr[++i])
	{
		tmp_cmd = ft_split(cmd->arg_arr[i], '=');
		aux_export(msh->envp, cmd->arg_arr[i], tmp_cmd);
	}
}
