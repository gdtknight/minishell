/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:50:42 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashmap.h"

/**
 * @brief 해시 맵에서 지정된 키를 가진 엔트리를 제거합니다.
 *
 * @details 
 * - `entry->key` 값을 기준으로 실제 해시 맵 내 엔트리를 탐색하여 제거합니다.
 * - 연결 리스트 형태로 구성된 해시 버킷에서 이전 노드와 다음 노드를 다시 연결합니다.
 * - 제거된 엔트리의 key/value 문자열과 엔트리 자체 메모리를 해제합니다.
 *
 * @note
 * - `entry` 포인터 자체가 해시 맵 내부 엔트리를 직접 가리키지 않아도 됩니다.
 * - key와 value는 동적 할당된 문자열이어야 하며, 본 함수에서 free()로 해제됩니다.
 *
 * @param map   해시 맵 포인터
 * @param entry 제거할 키를 가진 엔트리 포인터(키 값만 사용)
 */
void	remove_entry(t_hash_map *map, t_hash_entry *entry)
{
	int				idx;
	t_hash_entry	*target;

	if (!map || !entry || !entry->key)
		return ;
	idx = hash(entry->key);
	target = get_entry(map, entry->key);
	if (!target)
		return ;
	if (map->table[idx] == target)
		map->table[idx] = target->next;
	if (target->prev)
		target->prev->next = target->next;
	if (target->next)
		target->next->prev = target->prev;
	free(target->key);
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

	if (!map)
		return ;
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
		map->table[idx] = NULL;
	}
}
