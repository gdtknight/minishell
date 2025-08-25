/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:22:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:06:24 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file token_list.c
 * @brief Utilities for managing doubly-linked token lists.
 *
 * Provides functions to append, remove, clear, and access tokens
 * within a doubly-linked list structure. Handles proper memory
 * management for token values.
 */

#include <stdlib.h>
#include "def.h"
#include "tokenizer.h"

/**
 * @brief Free all nodes and values in a token list.
 *
 * Iterates over the list and frees each token's `value` and
 * the node itself. After this function, the list becomes empty.
 *
 * @param lst Pointer to the head of the token list.
 * @return t_result COMPLETED if successful, INCOMPLETED if `lst` is NULL.
 *
 * @note
 * - Each node's `value` is freed before freeing the node.
 * - The list pointer itself (`*lst`) is not set to NULL automatically.
 */
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

/**
 * @brief Append a token to the end of a token list.
 *
 * Adds the given token to the end of the list. If the list is empty,
 * the token becomes the first node.
 *
 * @param lst   Pointer to the head of the token list.
 * @param token Token to append.
 * @return t_result COMPLETED if successful, INCOMPLETED if `lst` is NULL.
 *
 * @note
 * - The new token's `prev` pointer is set to the previous last node.
 * - If the list is empty, `prev` remains NULL.
 */
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

/**
 * @brief Remove a specific token node from a token list.
 *
 * Disconnects the target node from the list while keeping the remaining
 * nodes linked.
 *
 * @param lst    Head of the token list.
 * @param target Token node to remove.
 * @return t_result COMPLETED if target is found and removed,
 *         INCOMPLETED if `lst` is NULL.
 *
 * @note
 * - Memory for `target` is not freed; the caller is responsible for freeing it.
 * - If `target` is not in the list, the function does nothing.
 * - Works correctly even if `target` is the first or last node.
 */
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
		if (cur->prev)
			cur->prev->next = cur->next;
		if (cur->next)
			cur->next->prev = cur->prev;
	}
	return (COMPLETED);
}

/**
 * @brief Get the last token in a token list.
 *
 * Traverses the list to return the last node.
 *
 * @param lst Pointer to the head of the token list.
 * @return t_token* Pointer to the last token, or NULL if the list is empty.
 */
t_token	*get_last_token(t_token	**lst)
{
	t_token	*cur;

	if (!lst || !*lst)
		return (NULL);
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	return (cur);
}
