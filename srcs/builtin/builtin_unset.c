/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/16 02:09:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"
#include "shell_data.h"

#include "builtin.h"

/**
 * @brief unset 빌트인 명령어를 실행하여 환경 변수(들)를 삭제한다.
 *
 * 이 함수는 인자로 전달된 각 이름에 해당하는 환경 변수 엔트리를
 * 셸의 환경 맵에서 찾아 제거한다. 존재하지 않는 키가 주어져도
 * 오류로 간주하지 않고 다음 인자를 계속 처리한다.
 *
 * 동작 방식:
 * - argc가 NULL 이거나 인자가 없는 경우(argc[1] 없음): 아무 것도 하지 않고 SUCCESS 반환
 * - 인자가 1개 이상인 경우: 각 인자에 대해 해시맵에서 엔트리를 찾아 삭제
 * - 실행이 끝나면 last_status를 0으로 설정
 *
 * @param argc  Null-terminated 문자열 배열
 *              - argc[0] : "unset"
 *              - argc[1..n] : 삭제할 환경 변수 이름들
 *
 * @return t_status
 *         - SUCCESS: 항상 성공 반환(존재하지 않는 키가 있어도 실패로 보지 않음)
 *
 * @note 현재 구현은 식별자 유효성(name_checker) 검증이나 에러 메시지 출력 없이
 *       조용히 삭제를 시도한다. bash 호환성을 강화하려면
 *       - 잘못된 식별자에 대한 오류 메시지 및 상태 코드 처리,
 *       - readonly/특수 변수 보호 규칙(필요 시),
 *       등을 추가할 수 있다.
 */
t_status	builtin_unset(char **argc)
{
	t_hash_map	*envp_map;
	int			i;

	envp_map = &get_shell_data()->envp_map;
	get_shell_data()->last_status = 0;
	if (!argc)
		return (SUCCESS);
	i = 1;
	while (argc[i])
	{
		remove_entry(envp_map, get_entry(envp_map, argc[i]));
		i++;
	}
	return (SUCCESS);
}
