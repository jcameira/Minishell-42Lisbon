/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:34:32 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/13 16:15:27 by mpais-go         ###   ########.fr       */
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

char	*find_slash(char *path)
{
	int		i;
	int		j;
	char	*new_path;

	i = -1;
	j = 0;
	while (path[++i])
	{
		if (path[i] == '/')
			j = i;
	}
	i = 0;
	new_path = malloc(sizeof (char) * (j + 1));
	while (i < j)
	{
		new_path[i] = path[i];
	}
	new_path[i] = '\0';
	free(path);
	return (new_path);
}
