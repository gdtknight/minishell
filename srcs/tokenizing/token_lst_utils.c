/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:22:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/17 18:19:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "debug.h"
#include "def.h"
#include "tokenizer.h"

t_status	clear_token_lst(t_token **lst)
{
	t_token	**head;
	t_token	*cur;

	if (!lst || !*lst)
		return (FAIL);
	head = lst;
	while (*lst)
	{
		cur = *lst;
		*lst = (*lst)->next;
		if (cur->value)
			free(cur->value);
		free(cur);
	}
	free(head);
	return (SUCCESS);
}

t_status	append_token_to_lst(t_token **lst, t_token *token)
{
	t_token	*last;

	if (!lst)
		return (FAIL);
	last = *lst;
	if (!last)
	{
		*lst = token;
		return (SUCCESS);
	}
	while (last->next)
		last = last->next;
	last->next = token;
	token->prev = last;
	return (SUCCESS);
}

t_status	insert_token_to_lst(t_token *lst, t_token *token)
{
	(void)lst;
	(void)token;
	return (SUCCESS);
}

t_status	remove_token_from_lst(t_token *lst, t_token *target)
{
	t_token	*cur;

	if (!lst)
		return (FAIL);
	cur = lst;
	while (cur)
	{
		if (cur != target)
			cur = cur->next;
		else
			break ;
	}
	if(cur) 
	{
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}
	return (SUCCESS);
}
