/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 03:30:13 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/31 18:35:17 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**arrcpy(char **dest, char **src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			free_arr(dest);
			free_arr(src);
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		}
		i++;
	}
	free_arr(src);
	return (dest);
}

char	**set_pwd(char **array)
{
	char	**new_arr;
	char	*cwd;
	int		size;

	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	if (array)
	{
		new_arr = arrcpy(new_arr, array);
		if (!new_arr)
			return (NULL);
	}
	cwd = getcwd(NULL, 0);
	new_arr[size] = ft_strjoin(PWD, cwd);
	free(cwd);
	if (!new_arr[size])
	{
		free_arr(new_arr);
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	new_arr[size + 1] = NULL;
	return (new_arr);
}

char	**set_shlvl(char **array)
{
	char	**new_arr;
	int		size;

	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	if (array)
	{
		new_arr = arrcpy(new_arr, array);
		if (!new_arr)
			return (NULL);
	}
	new_arr[size] = ft_strdup(SET_SHLVL);
	if (!new_arr[size])
	{
		free_arr(new_arr);
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	new_arr[size + 1] = NULL;
	return (new_arr);
}

char	**increment_shlvl(char **array)
{
	char	*tmp_str;
	char	*lvl;
	int		i;

	i = -1;
	tmp_str = NULL;
	while (array[++i])
	{
		if (!ft_strncmp(SHLVL, array[i], 6))
		{
			lvl = ft_itoa(ft_atoi(array[i] + 6) + 1);
			if (!lvl)
				return (ft_putstr_fd(NO_SPACE, 2), NULL);
			tmp_str = ft_strjoin(SHLVL, lvl);
			free(lvl);
			if (!tmp_str)
				return (ft_putstr_fd(NO_SPACE, 2), NULL);
			break ;
		}
	}
	free(array[i]);
	array[i] = ft_strdup(tmp_str);
	free(tmp_str);
	if (!array[i])
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	return (array);
}
