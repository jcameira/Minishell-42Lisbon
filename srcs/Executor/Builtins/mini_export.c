/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:59 by mpais-go          #+#    #+#             */
/*   Updated: 2024/10/02 22:09:45 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

void	print_declare(char **env)
{
	char	**tmp;
	int		i;

	i = -1;
	bubble_sort(env);
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "_=", 2))
			continue ;
		tmp = split_by_char(env[i], '=');
		printf("declare -x %s", tmp[0]);
		if (ft_strchr(env[i], '='))
			printf("=\"%s\"\n", tmp[1]);
		else
			printf("\n");
		free_arr(tmp);
	}
}

void	add_var_to_list(char *cmd_argarr, char ***list_to_add)
{
	char	**new_export;
	int		arr_size;

	new_export = malloc(sizeof(char *) * (array_size(*list_to_add) + 2));
	if (!new_export)
		return (ft_putstr_fd(NO_SPACE, 2));
	arr_size = array_size(*list_to_add);
	new_export = arrcpy(new_export, *list_to_add);
	if (!new_export)
		return ;
	new_export[arr_size] = ft_strdup(cmd_argarr);
	if (!new_export[arr_size])
		return (ft_putstr_fd(NO_SPACE, 2));
	new_export[arr_size + 1] = NULL;
	*list_to_add = new_export;
}

int	get_pos_in_list(char **list, char *target)
{
	char	**tmp_split;
	int		i;

	i = -1;
	while (list[++i])
	{
		tmp_split = split_by_char(list[i], '=');
		if (!ft_strcmp(target, tmp_split[0]))
			return (free_arr(tmp_split), i);
		free_arr(tmp_split);
	}
	return (-1);
}

void	aux_export(t_minishell *msh, char *cmd_argarr, char **tmp_cmd,
	char ***list)
{
	int	i;

	if (*list == msh->export_list)
	{
		i = get_pos_in_list(msh->export_list, tmp_cmd[0]);
		if (i == -1)
			add_var_to_list(cmd_argarr, list);
	}
	else
	{
		i = get_pos_in_list(msh->envp, tmp_cmd[0]);
		if (i == -1 && ft_strchr(cmd_argarr, '='))
			add_var_to_list(cmd_argarr, list);
	}
	if (i != -1)
	{
		if (tmp_cmd[1][0] || (!tmp_cmd[1][0] && ft_strchr(cmd_argarr, '=')))
		{
			free((*list)[i]);
			(*list)[i] = ft_strdup(cmd_argarr);
			if (!(*list)[i])
				return (free_arr(tmp_cmd), ft_putstr_fd(NO_SPACE, 2));
		}
	}
}

int	mini_export(t_minishell *msh, t_simplecmd *cmd)
{
	char	**tmp_cmd;
	int		i;

	if (cmd->arg_nbr == 1)
		print_declare(msh->export_list);
	i = 0;
	while (cmd->arg_arr[++i])
	{
		// printf("Argument -> %s\n", cmd->arg_arr[i]);
		tmp_cmd = split_by_char(cmd->arg_arr[i], '=');
		// if (!tmp_cmd[0])
		// {
		// 	free_arr(tmp_cmd);
		// 	continue ;
		// }
		if (!check_non_identifier_char(tmp_cmd, cmd->arg_arr[i]))
			return (EXIT_FAILURE);
		aux_export(msh, cmd->arg_arr[i], tmp_cmd, &msh->export_list);
		aux_export(msh, cmd->arg_arr[i], tmp_cmd, &msh->envp);
		free_arr(tmp_cmd);
	}
	return (EXIT_SUCCESS);
}
