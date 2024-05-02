/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:00:47 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/02 03:37:38 by jcameira         ###   ########.fr       */
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
	new_arr = malloc(sizeof(char *) * size);
	i = -1;
	while (array[++i])
		new_arr[i] = ft_strdup(array[i]);
	new_arr[i] = NULL;
	return (new_arr);
}

int	is_white_space(char *line)
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
