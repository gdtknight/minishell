/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 01:26:45 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashmap.h"

/**
 * @brief 해시 맵에서 지정된 엔트리를 제거합니다.
 *
 * @details 
 * - `entry->key`를 기준으로 실제 해시 맵에서 해당 엔트리를 찾아 제거합니다.
 * - 연결 리스트 형태의 해시 버킷에서 이전 노드와 다음 노드를 다시 연결합니다.
 * - `target->key`, `target->value`, `target`에 대해 메모리를 해제합니다.
 *
 * @note
 * - `entry` 자체는 해시 맵에 있는 기존 엔트리를 직접 가리키지 않아도 됩니다.
 * - 제거되는 엔트리의 key/value 메모리는 동적 할당된 문자열이어야 하며, 
 *   본 함수에서 free() 호출로 해제됩니다.
 *
 * @param map  해시 맵 포인터
 * @param entry 제거할 엔트리의 키 정보를 담은 엔트리 포인터
 */
void	remove_entry(t_hash_map *map, t_hash_entry *entry)
{
	t_hash_entry	*target;

	target = get_entry(map, entry->key);
	if (!target)
		return ;
	else if (map->table[hash(entry->key)] == target)
	{
		map->table[hash(entry->key)] = target->next;
		target->next->prev = (NULL);
	}
	else
	{
		if (target->prev)
			target->prev->next = target->next;
		if (target->next)
			target->next->prev = target->prev;
	}
	if (target->key)
		free(target->key);
	if (target->value)
		free(target->value);
	free(target);
}

/**
 * @brief 해시 맵 초기화(모든 엔트리 제거)
 *
 * 해시 맵에 저장된 모든 키-값 엔트리를 해제하고 테이블을 비운다.
 * 각 엔트리의 key와 value 메모리도 free 한다.
 *
 * @note
 * - put_key_value() / put_entry()를 통해 삽입된 key, value는
 *   반드시 동적 할당된 문자열이어야 하며, 여기서 free된다.
 *
 * @param map 초기화할 해시 맵
 */
void	clear_hashmap(t_hash_map *map)
{
	int				idx;
	t_hash_entry	*cur;
	t_hash_entry	*next;

	idx = -1;
	while (++idx < HASHTABLE_SIZE)
	{
		cur = map->table[idx];
		while (cur)
		{
			next = cur->next;
			if (cur->key)
				free(cur->key);
			if (cur->value)
				free(cur->value);
			free(cur);
			cur = next;
		}
	}
}

