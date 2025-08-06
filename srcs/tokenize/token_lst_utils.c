/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:22:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/06 18:14:11 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "def.h"
#include "tokenizer.h"

t_result	clear_token_lst(t_token **lst)
{
	t_token	*cur;

	if (!lst || !*lst)
		return (INCOMPLETED);
	while (*lst)
	{
		cur = *lst;
		*lst = (*lst)->next;
		if (cur->value)
			free(cur->value);
		free(cur);
	}
	return (COMPLETED);
}

t_result	append_token_to_lst(t_token **lst, t_token *token)
{
	t_token	*last;

	if (!lst)
		return (INCOMPLETED);
	last = *lst;
	if (!last)
	{
		*lst = token;
		return (COMPLETED);
	}
	while (last->next)
		last = last->next;
	last->next = token;
	token->prev = last;
	return (COMPLETED);
}

t_result	remove_token_from_lst(t_token *lst, t_token *target)
{
	t_token	*cur;

	if (!lst)
		return (INCOMPLETED);
	cur = lst;
	while (cur)
	{
		if (cur != target)
			cur = cur->next;
		else
			break ;
	}
	if (cur)
	{
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}
	return (COMPLETED);
}
