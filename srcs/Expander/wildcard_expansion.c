/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:28:03 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/24 15:54:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

char	*add_wildcard_content(char *content)
{
	char			*new_content;
	DIR				*directory;
	struct dirent	*file;

	directory = NULL;
	directory = get_directory_info(directory, &new_content, &file, 1);
	if (!new_content)
		return (NULL);
	while (file)
	{
		if (ft_strncmp(content, ".", 1) && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			new_content = append_more_wildcard_content(new_content, file);
			if (!new_content)
				return (NULL);
		}
		file = readdir(directory);
	}
	closedir(directory);
	return (new_content);
}

int	get_wildcards_len(char *content)
{
	int				real_len;
	DIR				*directory;
	struct dirent	*file;

	real_len = 0;
	directory = NULL;
	directory = get_directory_info(directory, NULL, &file, 0);
	while (file)
	{
		if (ft_strncmp(content, ".", 1) && file->d_name[0] == '.')
		{
			file = readdir(directory);
			continue ;
		}
		else if (match_wildcard_pattern(content, file->d_name))
		{
			if (real_len > 0)
				real_len++;
			real_len += ft_strlen(file->d_name);
		}
		file = readdir(directory);
	}
	closedir(directory);
	return (real_len);
}

int	wildcards_str_len(char *content)
{
	int	real_len;
	int	quotes[2];
	int	i;

	real_len = 0;
	quotes[S] = 0;
	quotes[D] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
		{
			real_len += get_wildcards_len(content);
			return (real_len);
		}
		real_len++;
	}
	return (real_len);
}

char	*expand_wildcards_aux(char *new_content, char *content)
{
	int		quotes[2];
	int		i;
	int		j;

	quotes[D] = 0;
	quotes[S] = 0;
	i = -1;
	j = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
		{
			new_content = add_wildcard_content(content);
			break ;
		}
		new_content[++j] = content[i];
	}
	free(content);
	return (new_content);
}

char	*expand_wildcards(char *content, int len, int needs_expansion)
{
	char	*new_content;

	new_content = NULL;
	if (!needs_expansion)
	{
		new_content = malloc(sizeof(char) * (len + 1));
		if (!new_content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		new_content[len] = '\0';
	}
	new_content = expand_wildcards_aux(new_content, content);
	if (!new_content)
		return (NULL);
	return (new_content);
}
