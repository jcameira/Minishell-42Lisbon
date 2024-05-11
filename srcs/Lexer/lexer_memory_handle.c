/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_memory_handle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:16:32 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/11 17:18:47 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

void	free_token_list(t_token_list *list)
{
	t_token_list	*tmp;

	while(list)
	{
		tmp = list->next;
		free(list->content);
		free(list);
		list = tmp;
	}
}
