/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:23:06 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/03 17:46:37 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

void	open_subshell(char *line)
{
	int		i;
	int		len;
	int		start;
	int		parentesis;
	char	*subline;

	len = 0;
	parentesis = 0;
	i = 0;
	subline = NULL;
	while (line[i])
	{
		if (line[i] == '(')
		{
			parentesis++;
			start = i + 1;
		}
		i++;
		while (line[i] && parentesis > 0)
		{
			len++;
			if (line[i] == '(')
				parentesis++;
			if (line[i] == ')')
			{
				parentesis--;
				printf("%d\n", parentesis);
			}
			i++;
		}
		if (start && len)
		{
			printf("Start -> %d\nSize -> %d\n", start ,len);
			subline = ft_substr(line, start, --len);
			printf("%s\n", subline);
			//lexer(subline);
			free(subline);
			start = 0;
			len = 0;
		}
	}
	printf("%s\n", line);
}
