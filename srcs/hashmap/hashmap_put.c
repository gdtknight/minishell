/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:23:22 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 02:03:43 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

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
		if (entry->value != NULL)
			free(entry->value);
		entry->value = ft_strdup(value);
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
