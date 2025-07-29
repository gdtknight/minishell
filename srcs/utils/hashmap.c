/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/29 23:05:28 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashmap.h"

/**
 * @brief 해시 맵에 키-값 쌍 저장
 *
 * 주어진 키가 이미 존재하면 해당 엔트리의 값을 새로운 값으로 교체한다.
 * 존재하지 않는 경우 새로운 해시 엔트리를 생성하여 해시 맵에 삽입한다.
 *
 * @note
 * - key와 value는 동적으로 할당된 문자열이어야 한다.
 * - clear_hashmap() 호출 시 key와 value 메모리가 free된다.
 *
 * @param map   값을 저장할 해시 맵
 * @param key   삽입 또는 갱신할 키 문자열(동적 할당 필요)
 * @param value 삽입 또는 갱신할 값 문자열(동적 할당 필요)
 */
void	put_key_value(t_hash_map *map, char *key, char *value)
{
	t_hash_entry	*entry;

	entry = get_entry(map, key);
	if (entry)
	{
		/* 기존 값 교체 (기존 value free 여부는 정책에 따라 다르게 처리 가능) */
		entry->value = value;
		return ;
	}
	/* 새 엔트리 생성 및 삽입 */
	entry = create_new_entry(key, value);
	put_entry(map, entry);
}

/**
 * @brief 해시 맵에 엔트리 삽입(삽입 전용)
 *
 * 동일 키 존재 여부는 검사하지 않으며, 단순히 해당 버킷 끝에 엔트리를 연결한다.
 * 중복 키 검사 및 업데이트는 put_key_value()에서 수행한다.
 *
 * @note
 * - entry->key와 entry->value는 동적으로 할당된 문자열이어야 한다.
 * - clear_hashmap() 호출 시 key와 value 메모리가 free된다.
 *
 * @param map   엔트리를 삽입할 해시 맵
 * @param entry 삽입할 해시 엔트리 포인터
 */
void	put_entry(t_hash_map *map, t_hash_entry *entry)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(entry->key);
	cur = (map->table)[idx];
	if (!cur)
	{
		(map->table)[idx] = entry;
		return ;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = entry;
	entry->prev = cur;
}

/**
 * @brief 주어진 키에 해당하는 엔트리 조회
 *
 * 키를 해싱하여 대응되는 버킷을 찾고, 연결 리스트를 순회하면서
 * 해당 키와 동일한 엔트리를 찾아 반환한다.
 *
 * @note
 * - key 문자열은 NULL이 아니어야 한다.
 * - 반환된 엔트리는 해시 맵 내부 메모리이므로 free 하면 안 된다.
 *
 * @param map 조회 대상 해시 맵
 * @param key 조회할 키 문자열
 * @return 키에 해당하는 t_hash_entry 포인터, 없으면 NULL 반환
 */
t_hash_entry	*get_entry(t_hash_map *map, char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur)
	{
		if (equal_key(cur->key, key))
			return (cur);
		cur = cur->next;
	}
	return (NULL);
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

