/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:24:17 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 02:03:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashmap.h"

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
t_hash_entry	*get_entry(t_hash_map *map, const char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur != NULL)
	{
		if (equal_key(cur->key, key))
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/**
 * @brief 주어진 키(key)에 해당하는 값을 조회합니다.
 *
 * 해시 함수로 버킷(bucket) 인덱스를 계산한 뒤,
 * 해당 버킷에 연결된 엔트리들을 순회하면서 키를 비교하여
 * 일치하는 값(value)을 찾습니다.
 *
 * @param map 값을 조회할 대상 해시맵의 포인터
 * @param key 조회할 키 (NULL 아님)
 * @return char* 키에 해당하는 값의 포인터.
 *         키가 존재하지 않으면 NULL 반환.
 *
 * @note 반환되는 값 포인터는 해시맵 내부 메모리를 가리키며,
 *       호출자가 해제(free)하면 안 됩니다.
 */
char	*get_value(t_hash_map *map, const char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur)
	{
		if (equal_key(cur->key, key))
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
