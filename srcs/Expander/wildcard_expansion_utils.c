/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:41:41 by jcameira          #+#    #+#             */
/*   Updated: 2024/09/27 00:55:56 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expander.h>

int	match_wildcard_pattern(char *pattern, char *file)
{
	if (!(*pattern) && !(*file))
		return (1);
	if (*pattern == '*')
		return (match_wildcard_pattern(pattern + 1, file)
			|| (*file && match_wildcard_pattern(pattern, file + 1)));
	if (*pattern == *file)
		return (match_wildcard_pattern(pattern + 1, file + 1));
	return (0);
}

int	needs_wildcard_expansion(char *content)
{
	int	expanded_len;
	int	quotes[2];
	int	i;

	quotes[D] = 0;
	quotes[S] = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !quotes[D])
			quotes[S] = !quotes[S];
		else if (content[i] == '"' && !quotes[S])
			quotes[D] = !quotes[D];
		else if (content[i] == '*' && !quotes[S] && !quotes[D])
		{
			expanded_len = get_wildcards_len(content);
			if (expanded_len)
				return (1);
		}
	}
	return (0);
}

DIR	*get_directory_info(DIR *directory, char **new_content,
	struct dirent **file, int flag)
{
	char	*cwd;

	if (flag)
	{
		*new_content = malloc(sizeof(char));
		if (!(*new_content))
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		(*new_content)[0] = '\0';
	}
	cwd = getcwd(NULL, 0);
	directory = opendir(cwd);
	free(cwd);
	*file = readdir(directory);
	return (directory);
}

char	*append_more_wildcard_content(char *new_content, struct dirent *file)
{
	char			*tmp;

	if (new_content[0])
	{
		tmp = ft_strdup(new_content);
		if (!tmp)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		free(new_content);
		new_content = ft_strjoin(tmp, " ");
		if (!new_content)
			return (ft_putstr_fd(NO_SPACE, 2), NULL);
		free(tmp);
	}
	tmp = ft_strdup(new_content);
	if (!tmp)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free(new_content);
	new_content = ft_strjoin(tmp, file->d_name);
	if (!new_content)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	free(tmp);
	return (new_content);
}
