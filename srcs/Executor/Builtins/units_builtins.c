/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:34:32 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/23 11:02:44 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	**get_new_strs(char **new_strs, char *str, char c)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			i++;
		else
		{
			j = 0;
			while (str[i + j] && str[i + j] != c)
				j++;
			new_strs[0] = ft_substr(str, i, j);
			if (!new_strs[0])
				return (free_arr(new_strs), NULL);
			i += j + 1;
			break ;
		}
	}
	new_strs[1] = ft_substr(str, i, ft_strlen(str) - i);
	if (!new_strs[1])
		return (free_arr(new_strs), NULL);
	return (new_strs);
}

char	**split_by_char(char *str, char c)
{
	char	**new_strs;

	new_strs = (char **)malloc(sizeof (char *) * 3);
	if (!new_strs)
		return (NULL);
	new_strs = get_new_strs(new_strs, str, c);
	new_strs[2] = NULL;
	return (new_strs);
}

char	*find_path(t_minishell *msh, t_simplecmd *cmd, char *target)
{
	char	*final_path;
	char	*tmp;
	int		lenght;
	int		i;
	int		j;

	i = -1;
	lenght = ft_strlen(target);
	(void)cmd;
	while (msh->envp[++i])
	{
		if (!ft_strncmp(msh->envp[i], target, lenght))
		{
			tmp = msh->envp[i];
			j = -1;
			while (++j < lenght)
				tmp++;
			final_path = ft_strdup(tmp);
			if (!final_path)
				return (ft_putstr_fd(NO_SPACE, 2), NULL);
			return (final_path);
		}
	}
	return (NULL);
}

char	*parent_dir(char *path)
{
	int		i;
	int		j;
	char	*new_path;

	i = ft_strlen(path);
	while (i >= 0)
		if (path[--i] == '/')
			break ;
	if (i == 0)
	{
		new_path = malloc(sizeof (char) * 2);
		new_path[0] = path[0];
		new_path[1] = '\0';
		return (new_path);
	}
	new_path = malloc(sizeof (char) * (i + 1));
	j = -1;
	while (++j < i)
	{
		new_path[j] = path[j];
	}
	new_path[j] = '\0';
	return (new_path);
}
