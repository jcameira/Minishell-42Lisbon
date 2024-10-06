/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:24:31 by jcameira          #+#    #+#             */
/*   Updated: 2024/10/05 22:25:02 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

void	refine_cmd_arg_arr(char ***content, char **tmp, char **new_content,
	int i)
{
	int	index[2];

	index[0] = -1;
	index[1] = -1;
	while ((*content)[++index[0]])
	{
		if (index[0] == i)
		{
			i = -1;
			while (tmp[++i])
			{
				new_content[++index[1]] = ft_strdup(tmp[i]);
				if (!new_content[index[1]])
					return (free_arr(new_content), free_arr(tmp),
						ft_putstr_fd(NO_SPACE, 2));
			}
		}
		else
		{
			new_content[++index[1]] = ft_strdup((*content)[index[0]]);
			if (!new_content[index[1]])
				return (free_arr(new_content), free_arr(tmp),
					ft_putstr_fd(NO_SPACE, 2));
		}
	}
}

void	word_split_cleanup(char **str, char **tmp, char **new_content,
	int new_size)
{
	free((*str));
	(*str) = ft_strdup(tmp[0]);
	if (!(*str))
		return (ft_putstr_fd(NO_SPACE, 2));
	free_arr(tmp);
	new_content[new_size - 1] = NULL;
}

char	*word_split(char *str, char ***content, int i)
{
	char	**new_content;
	char	**tmp;
	int		new_size;

	if (!space_not_in_quotes(str))
		return (str);
	tmp = ft_split(str, ' ');
	if (!tmp)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	if (array_size(tmp) == 1)
		return (str);
	new_size = array_size(*content) + array_size(tmp);
	new_content = malloc(sizeof(char *) * new_size);
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	refine_cmd_arg_arr(content, tmp, new_content, i);
	if (!new_content)
		return (NULL);
	word_split_cleanup(&str, tmp, new_content, new_size);
	if (!str)
		return (NULL);
	free_arr(*content);
	*content = new_content;
	return (str);
}
