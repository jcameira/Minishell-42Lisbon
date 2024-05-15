/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_creation_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:14:11 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/15 14:05:01 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_token_list	*new_token(int type, char *data)
{
	t_token_list	*new;

	new = malloc(sizeof(t_token_list));
	if (!new)
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	new->token_type = type;
	new->content = ft_strdup(data);
	if (!new->content)
	{
		free(new);
		return (ft_putstr_fd(NO_SPACE, 2), NULL);
	}
	new->next = NULL;
	return (new);
}

void	add_new_token(t_token_list **token_list, t_token_list *new)
{
	t_token_list	*last;

	if (!token_list)
		return ;
	if (!*token_list)
	{
		*token_list = new;
		return ;
	}
	last = last_token(*token_list);
	last->next = new;
}

void	add_token_middle_list(t_token_list **token_list, t_token_list *new)
{
	t_token_list	*tmp;

	if (!token_list)
		return ;
	if (!*token_list)
		*token_list = new;
	tmp = (*token_list)->next;
	(*token_list)->next = new;
	last_token(new)->next = tmp;
}

t_token_list	*last_token(t_token_list *token_list)
{
	t_token_list	*tmp;

	if (!token_list)
		return (NULL);
	tmp = token_list;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_token_list	*set_token_list_types(t_token_list *token_list)
{
	t_token_list	*tmp;

	if (!token_list)
		return (NULL);
	tmp = token_list;
	while (tmp)
	{
		tmp->token_type = set_token_type(tmp->content);
		tmp = tmp->next;
	}
	return (token_list);
}
