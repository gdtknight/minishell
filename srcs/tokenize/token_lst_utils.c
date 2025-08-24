/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:22:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/13 18:33:51 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "def.h"
#include "tokenizer.h"

/**
 * @brief 토큰 리스트 전체를 해제한다.
 *
 * 주어진 토큰 리스트 포인터(`lst`)를 순회하며 각 노드와
 * `value` 문자열을 메모리에서 해제한다. 모든 노드가 해제되면
 * 리스트는 비어 있게 된다.
 *
 * @param lst 토큰 리스트의 시작 노드 포인터의 주소
 * @return t_result COMPLETED(성공), INCOMPLETED(NULL 인자가 전달된 경우)
 *
 * @note
 * - 각 노드의 `value`가 NULL이 아니면 먼저 `free()`로 해제한 후 노드를 해제한다.
 * - 리스트 포인터 자체(`*lst`)는 호출자가 별도로 NULL로 초기화할 수 있다.
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
 * @brief 토큰을 토큰 리스트의 끝에 추가한다.
 *
 * 주어진 토큰(`token`)을 토큰 리스트(`lst`)의 마지막에 연결한다.
 * 리스트가 비어 있으면 첫 번째 노드로 설정한다.
 *
 * @param lst    토큰 리스트의 시작 노드 포인터의 주소
 * @param token  추가할 토큰 노드 포인터
 * @return t_result COMPLETED(성공), INCOMPLETED(NULL 인자가 전달된 경우)
 *
 * @note
 * - 새 토큰의 `prev` 포인터는 기존 마지막 노드를 가리키도록 설정된다.
 * - 리스트가 비어 있으면 `prev`는 NULL로 유지된다.
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
 * @brief 토큰 리스트에서 특정 토큰 노드를 제거한다.
 *
 * 주어진 리스트(`lst`)에서 `target` 노드를 찾아 제거한다.
 * 이전 노드와 다음 노드를 서로 연결하여 리스트의 연속성을 유지한다.
 *
 * @param lst     토큰 리스트의 시작 노드 포인터
 * @param target  제거할 토큰 노드 포인터
 * @return t_result COMPLETED(성공), INCOMPLETED(NULL 인자가 전달된 경우)
 *
 * @note
 * - 노드 제거 시 메모리 해제는 수행하지 않는다. 해제는 호출자가 직접 해야 한다.
 * - `target`이 리스트에 없으면 아무 동작도 하지 않는다.
 * - `target`이 리스트의 첫 번째나 마지막 노드인 경우에도 동작한다.
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
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}
	return (COMPLETED);
}

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
