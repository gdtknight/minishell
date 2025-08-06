/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/01 19:03:18 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "def.h"
#include "hashmap.h"
#include "builtin.h"

/**
 * @brief 해시맵에 저장된 모든 환경 변수를 출력한다.
 *
 * 해시맵의 모든 버킷을 순회하며 key=value 형태로
 * 표준 출력(stdout)에 출력한다.
 *
 * @param map  환경 변수가 저장된 해시맵
 */
t_status	builtin_env(t_hash_map map)
{
	t_hash_entry	*current;
	int				i;

	i = 0;
	while (i < HASHTABLE_SIZE)
	{
		current = map.table[i];
		while (current)
		{
			printf("%s=%s\n", current->key, current->value);
			current = current->next;
		}
		i++;
	}
	return (SUCCESS);
}
