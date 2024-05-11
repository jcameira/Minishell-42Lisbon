/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:00:47 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/11 20:05:51 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	array_size(char **array)
{
	char	**tmp_arr;
	int		size;

	if (!array)
		return (0);
	tmp_arr = array;
	size = 0;
	while (tmp_arr[size])
		size++;
	return (size);
}

char	**arrdup(char **array)
{
	char	**new_arr;
	int		size;
	int		i;

	if (!(*array))
		return (NULL);
	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 1));
	if (!new_arr)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	i = -1;
	while (array[++i])
	{
		new_arr[i] = ft_strdup(array[i]);
		if (!new_arr[i])
		{
			free_arr(new_arr);
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		}
	}
	new_arr[i] = NULL;
	return (new_arr);
}

int	full_white_space(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if ((line[i] >= 9 && line[i] <= 13) || line[i] == 32)
			continue ;
		else
			return (0);
	}
	return (1);
}