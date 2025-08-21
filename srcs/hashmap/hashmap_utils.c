/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 02:03:03 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "hashmap.h"

/**
 * @brief 새로운 해시 엔트리 생성
 *
 * 주어진 key와 value를 가지는 새로운 해시 엔트리를 생성한다.
 * 메모리는 malloc으로 동적 할당되며, next와 prev 포인터는 NULL로 초기화된다.
 *
 * @param key   해시 엔트리의 키 문자열 (NULL이 아님을 가정)
 * @param value 해시 엔트리의 값 문자열 (NULL이 아님을 가정)
 * @return 생성된 t_hash_entry 포인터, 메모리 할당 실패 시 NULL 반환
 */
t_hash_entry	*create_new_entry(char *key, char *value)
{
	t_hash_entry	*new_entry;

	new_entry = (t_hash_entry *) malloc(sizeof(t_hash_entry));
	if (!new_entry)
		return (NULL);
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = NULL;
	new_entry->prev = NULL;
	return (new_entry);
}

/**
 * @brief 해시맵에 저장된 전체 엔트리 개수를 계산합니다.
 *
 * 해시 테이블의 모든 버킷(bucket)을 순회하면서 연결 리스트로 체이닝된
 * 모든 엔트리를 탐색하여 개수를 계산합니다.
 *
 * @param map 엔트리 개수를 계산할 대상 해시맵의 포인터
 * @return int 해시맵에 저장된 엔트리 총 개수
 */
int	count_entry(t_hash_map *map)
{
	int				cnt;
	int				idx;
	t_hash_entry	*cur;

	cnt = 0;
	idx = -1;
	while (++idx < DEFAULT_HASHTABLE_SIZE)
	{
		cur = (map->table)[idx];
		while (cur)
		{
			cnt++;
			cur = cur->next;
		}
	}
	return (cnt);
}

/**
 * @brief 주어진 키가 해시 맵에 존재하는지 확인
 *
 * 해시 함수를 이용해 키가 저장된 버킷을 찾아 선형 탐색한다.
 *
 * @param map 검색 대상 해시 맵
 * @param key 존재 여부를 확인할 키 문자열
 * @return TRUE(존재함), FALSE(존재하지 않음)
 */
t_boolean	contains_key(t_hash_map *map, char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur)
	{
		if (equal_key(cur->key, key))
			return (TRUE);
		cur = cur->next;
	}
	return (FALSE);
}

/**
 * @brief 두 키 문자열이 동일한지 비교
 *
 * ori_key와 target_key를 비교하여 동일하면 TRUE를 반환한다.
 * 내부적으로 ft_strncmp를 이용해 ori_key 길이 기준으로 비교한다.
 *
 * @param ori_key   원본 키 문자열
 * @param target_key 비교 대상 키 문자열
 * @return TRUE(동일), FALSE(다름)
 */
t_boolean	equal_key(char *ori_key, char *target_key)
{
	if (!ori_key || !target_key)
		return (FALSE);
	return (ft_strlen(ori_key) == ft_strlen(target_key)
		&& ft_strncmp(ori_key, target_key, ft_strlen(ori_key)) == 0);
}

/**
 * @brief 문자열 키 해시값 계산
 *
 * 주어진 문자열 키를 이용해 HASHTABLE_SIZE 범위 내의 해시 값을 계산한다.
 * 단순한 31배수 가중치 합산 해시 함수를 사용한다.
 *
 * @param key 해시값을 계산할 문자열 키
 * @return 계산된 해시값 (0 이상 HASHTABLE_SIZE 미만)
 */
int	hash(const char *key)
{
	int	hash;

	hash = 0;
	while (*key)
		hash = ((hash + (*key++) * 31) % DEFAULT_HASHTABLE_SIZE);
	return (hash);
}
