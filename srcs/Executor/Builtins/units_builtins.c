/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:34:32 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/12 15:52:16 by jcameira         ###   ########.fr       */
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
