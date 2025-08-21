/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/20 18:10:55 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"
#include "hashmap.h"
#include "builtin.h"
#include "shell.h"

/**
 * @brief env 빌트인 명령어를 실행하여 환경 변수를 출력한다.
 *
 * 이 함수는 현재 셸의 환경 변수 목록을 전부 출력한다.
 * 각 환경 변수는 `키=값` 형식으로 한 줄씩 출력되며,
 * 해시맵의 모든 버킷을 순회하며 값을 가져온다.
 *
 * 동작 방식:
 * - argc 인자는 사용하지 않음
 * - envp_map 해시맵의 모든 인덱스를 순회
 * - 각 버킷의 연결 리스트를 따라 모든 key-value 쌍을 출력
 * - 실행이 끝나면 last_status를 0으로 설정
 *
 * @param argc  명령어와 인자 배열 (env는 인자를 사용하지 않음)
 *
 * @return t_status
 *         - SUCCESS: 항상 성공 반환
 */
t_status	builtin_env(char **argc)
{
	t_hash_map		map;
	t_hash_entry	*current;
	int				i;

	(void) argc;
	map = get_shell_data()->envp_map;
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
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}
