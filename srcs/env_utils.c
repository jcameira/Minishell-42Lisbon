/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:30:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/02 03:30:55 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**set_pwd(char **array)
{
	char	**new_arr;
	char	*cwd;
	int		size;
	int		i;

	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 1));
	i = 0;
	if (array)
	{
		while (array[i])
		{
			new_arr[i] = ft_strdup(array[i]);
			free(array[i]);
			i++;
		}
	}
	free(array);
	cwd = getcwd(NULL, 0);
	new_arr[i] = ft_strjoin(PWD, ft_strdup(cwd));
	free(cwd);
	new_arr[++i] = NULL;
	return (new_arr);
}

char	**set_shlvl(char **array)
{
	char	**new_arr;
	int		size;
	int		i;

	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 1));
	i = 0;
	if (array)
	{
		while (array[i])
		{
			new_arr[i] = ft_strdup(array[i]);
			free(array[i]);
			i++;
		}
	}
	free(array);
	new_arr[i] = ft_strdup(SET_SHLVL);
	new_arr[++i] = NULL;
	return (new_arr);
}

char	**increment_shlvl(char **array)
{
	char	*tmp_str;
	char	*lvl;
	int		i;

	i = -1;
	lvl = NULL;
	while (array[++i])
	{
		if (!ft_strncmp(SHLVL, array[i], 6))
		{
			lvl = ft_itoa(ft_atoi(array[i] + 6) + 1);
			break ;
		}
	}
	tmp_str = ft_strjoin(SHLVL, lvl);
	free(array[i]);
	if (lvl)
		free(lvl);
	array[i] = ft_strdup(tmp_str);
	free(tmp_str);
	return (array);
}
