/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpais-go <mpais-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:47:59 by mpais-go          #+#    #+#             */
/*   Updated: 2024/05/11 15:19:30 by mpais-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

char	**add_env(char **array, char *new_env)
{
	char	**new_arr;
	int		size;
	int		i;

	if (!(*array))
		return (NULL);
	size = array_size(array);
	new_arr = malloc(sizeof(char *) * (size + 2));
	i = -1;
	while (array[++i])
		new_arr[i] = ft_strdup(array[i]);
	new_arr[i] = ft_strdup(new_env);
	new_arr[i + 1] = NULL;
	free_arr(array);
	return (new_arr);
}

int	number_lines(char **matrix)
{
	int 	i;
	
	i = 0;
	while(matrix[i])
		i++;
	return (i);
}

void	create_copy(char **matrix, char **env)
{
	int		i;
	int		j;

	i = -1;
	while (++i < number_lines(env))
		matrix[i] = ft_strdup(env[i]);
	matrix[i] = NULL;
}

void	print_declare(char **env)
{
	char	**copy_env;
	char	**tmp;
	int		i;

	i = -1;
	copy_env = malloc(sizeof(char *) * (number_lines(env) + 1));
	create_copy(copy_env, env);
	bubble_sort(copy_env);
	while (copy_env[++i])
	{
		if (!ft_strncmp(copy_env[i], "_=", 2))
			continue ;
		tmp = ft_split(copy_env[i], "=");
		printf("declare -x %s", tmp[0]);
		if(!tmp[1])
			printf("\n");
		else
			printf("=\"%s\"\n", tmp[1]);
		free_arr(tmp);
	}
}

void	bubble_sort(char **matrix)
{
	int		i;
	int		j;
	char	*tmp;
	int		size;

	size = 0;
	while (matrix[size])
		size++;
	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(matrix[j], matrix[j + 1]) > 0)
			{
				tmp = matrix[j];
				matrix[j] = matrix[j + 1];
				matrix[j + 1] = tmp;
			}
		}
	}
}

// void	mini_export(t_minishell *msh, t_simplecmd *cmd)
// {
// 	int		i;
// 	char 	**tmp1;
// 	char	**tmp2;
	
// 	i = -1;
// 	tmp2 = ft_split(cmd->arg_arr[1], "=");
// 	while (msh->envp[++i])
// 	{
// 		tmp1 = ft_split(msh->envp[i], "=");
// 		if (!ft_strncmp(tmp2[0], tmp1[0], ft_strlen(tmp1[0])) && tmp2[1])
// 		{
// 			free(msh->envp[i]);
// 			msh->envp[i] = ft_strdup(cmd->arg_arr[1]);
// 			free_arr(tmp1);
// 			free_arr(tmp2);
// 			return ;
// 		}
// 		free_arr(tmp1);
// 	}
// 	free_arr(tmp2);
// 	msh->envp = add_env(&msh->envp[i], cmd->arg_arr[1]);
// 	//se nao tiver nada, so export tem de aparecer tudo com declare -x antes e com aspas depois do =
// 	// e por ordem alfabetica o env. atencao ao exemplo do a sem =, nao aparece no env mas aparece no export
// }

void	mini_export(t_minishell *msh, t_simplecmd *cmd)
{
	char	**tmp_cmd;
	char	**tmp_env;
	int		i;
	int		j;
	
	if (cmd->arg_nbr == 1)
		print_declare(msh->envp);
	i = 0;
	while (cmd->arg_arr[++i])
	{
		tmp_cmd = ft_split(cmd->arg_arr[i], "=");
		j = -1;
		while (msh->envp[++j])
		{
			tmp_env = ft_split(msh->envp[++j], "=");
			if (!tmp_cmd[1] && !ft_strncmp(tmp_cmd[0], tmp_env[0], ft_strlen(tmp_cmd[0])))
				break;
			if (!msh->envp[j + 1] || (tmp_cmd[1] && !ft_strncmp(tmp_cmd[0], tmp_env[0], ft_strlen(tmp_cmd[0]))))
			{
				free(msh->envp[j]);
				msh->envp[j] = ft_strdup(cmd->arg_arr[i]);
				free_arr(tmp_cmd);
				free_arr(tmp_env);
			}
		}
		
	}
		
}

//o joao diz para copiar o env para outra lista porque e um chato do caralho.
//assim ha lista para env e lista para export. eu discordo.