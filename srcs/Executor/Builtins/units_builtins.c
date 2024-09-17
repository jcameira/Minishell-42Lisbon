/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:34:32 by mpais-go          #+#    #+#             */
/*   Updated: 2024/09/17 16:38:21 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	*find_path(t_minishell *msh, t_simplecmd *cmd, char *macro)
{
	char	*tmp;
	int		lenght;
	int		i;
	int		j;

	i = -1;
	lenght = ft_strlen(macro);
	(void)cmd;
	while (msh->envp[++i])
	{
		if (!ft_strncmp(msh->envp[i], macro, lenght))
		{
			tmp = msh->envp[i];
			j = -1;
			while (++j < lenght)
				tmp++;
			return (tmp);
		}
	}
	return (NULL);
}

char	*find_last_slash(char *path)
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
