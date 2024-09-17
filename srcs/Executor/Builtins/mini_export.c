/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:59 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/17 21:05:44 by jcameira         ###   ########.fr       */
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
	char	**tmp;
	int		i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "_=", 2))
			continue ;
		tmp = ft_split(env[i], '=');
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

char	**get_new_strs(char **new_strs, char *str, char c)
{
	int	i;
	int	j;
	int k;

	i = 0;
	k = -1;
	while (str[i])
	{
		if (str[i] && (str[i] == c))
			i++;
		else
		{
			j = 0;
			while (str[i + j] && str[i + j] != c)
				j++;
			if (k < 1)
			{
				new_strs[++k] = ft_substr(str, i, j);
				if (!new_strs[k])
					return (free_arr(new_strs), NULL);
			}
			i += j;
		}
	}
	printf("%s %s\n", new_strs[0], new_strs[1]);
	return (new_strs);
}

char	**split_by_char(char *str, char c)
{
	char	**new_strs;

	new_strs = (char **)malloc(sizeof (char *) * 3);
	if (!new_strs)
		return (NULL);
	new_strs = get_new_strs(new_strs, str, c);
	new_strs[2] = 0;
	return (new_strs);
}

void	mini_export(t_minishell *msh, t_simplecmd *cmd)
{
	char	**tmp_cmd;
	int		i;

	if (cmd->arg_nbr == 1)
		print_declare(msh->export_list);
	i = 0;
	while (cmd->arg_arr[++i])
	{
		//tmp_cmd = ft_split(cmd->arg_arr[i], '=');
		tmp_cmd = split_by_char(cmd->arg_arr[i], '=');
		aux_export(msh->envp, cmd->arg_arr[i], tmp_cmd);
	}
}
