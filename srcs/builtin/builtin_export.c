/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/16 23:41:20 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "hashmap.h"
#include "def.h"
#include "libft.h"
#include "builtin.h"
#include "shell_data.h"
#include "utils.h"

/**
 * @brief 환경 변수 이름의 유효성을 검사한다.
 *
 * 환경 변수의 이름이 유효한지 판단한다.
 * 첫 글자는 알파벳 또는 밑줄(_)이어야 하고,
 * 이후 문자는 알파벳, 숫자, 밑줄만 허용된다.
 *
 * @param name  검사할 환경 변수 이름
 * @return t_boolean
 *         - TRUE  : 유효한 환경 변수 이름
 *         - FALSE : 잘못된 이름
 */
t_boolean	name_checker(char *name)
{
	int	i;

	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (FALSE);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * @brief 환경 변수 문자열을 파싱하여 환경 변수 맵에 저장한다.
 *
 * "KEY=VALUE" 형식의 문자열을 파싱하여 KEY와 VALUE를 추출한 뒤,
 * KEY가 유효하면 환경 변수 맵에 저장하고,
 * 유효하지 않으면 에러 메시지를 출력한다.
 *
 * @param envp  "KEY=VALUE" 형식의 문자열
 * @param map   환경 변수를 저장하는 해시 맵
 * @return t_status
 *         - SUCCESS: 저장 성공
 *         - FAILURE: KEY 이름이 유효하지 않음
 */
t_status	check_and_set_enp(char *envp, t_hash_map *map)
{
	char	*key;
	char	*value;
	int		status;

	status = SUCCESS;
	key = extract_key(envp);
	value = extract_value(envp);
	if (name_checker(key))
	{
		if (ft_strnchr(envp, '=', ft_strlen(envp)) == -1)
		{
			free (key);
			free (value);
			return (status);
		}
		put_key_value(map, key, value);
	}
	else
	{
		printf("bash: export: `%s': not a valid identifier\n", key);
		status = ERROR;
	}
	free (key);
	free (value);
	return (status);
}

/**
 * @brief export 빌트인 명령어를 실행한다.
 *
 * 주어진 인자 목록을 기반으로 환경 변수를 추가 또는 수정한다.
 * 각 인자는 "KEY=VALUE" 형식을 따라야 하며,
 * KEY 이름이 유효하지 않으면 에러 메시지를 출력하고 상태 코드를 1로 설정한다.
 *
 * 동작 방식:
 * - 인자가 없는 경우: (현재 구현에서는 단순히 종료, 나중에 정렬 출력 기능 추가 가능)
 * - 인자가 있는 경우: 각 인자를 `check_and_set_enp()`로 처리
 * - 실패한 인자가 있으면 `last_status`를 1로 설정
 *
 * @param argc  Null-terminated 문자열 배열
 *              - argc[0] : "export"
 *              - argc[1..n] : "KEY=VALUE" 형식의 환경 변수
 *
 * @return t_status
 *         - SUCCESS: 모든 인자 처리 성공
 *         - FAILURE: 하나 이상 처리 실패
 */
t_status	builtin_export(char **argc)
{
	t_hash_map	*envp_map;
	t_status	status;
	int			i;

	envp_map = &get_shell_data()->envp_map;
	status = SUCCESS;
	get_shell_data()->last_status = 0;
	i = 1;
	while (argc[i])
	{
		if (check_and_set_enp(argc[i], envp_map) == ERROR)
		{
			status = ERROR;
			get_shell_data()->last_status = 1;
		}
		i++;
	}
	return (status);
}
