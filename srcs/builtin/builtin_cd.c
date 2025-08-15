/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:09 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/15 20:36:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "def.h"
#include "hashmap.h"
#include "shell_data.h"
#include "builtin.h"

int	count_argc(char **argc)
{
	int	i;

	i = 0;
	while (argc[i])
		i++;
	return (i);
}

/**
 * @brief cd 인자 및 errno를 기반으로 오류 유형을 판별한다.
 *
 * 규칙(목표: bash 호환):
 * - 인자가 없음 또는 "cd --" → HOME로 이동 시도. HOME 미설정이면 CD_HOME_NOT_SET.
 * - 인자가 "-" → OLDPWD로 이동 시도. OLDPWD 미설정이면 CD_OLDPWD_NOT_SET.
 * - 그 외 경로 인자일 때 chdir 실패의 errno를 매핑(EACCES/ENOENT/ENOTDIR).
 *
 * @param argc NULL-terminated argv (argc[0] = "cd")
 * @return t_cd_err 판별된 오류 코드
 *
 * @note
 * - errno 기반 분기는 chdir() 실패 직후에만 신뢰 가능하다.
 * - HOME/OLDPWD 존재 여부를 확인하려면 호출 측에서 환경 변수를 검사해
 *   별도의 에러 코드를 먼저 넘겨주는 편이 안전하다(아래 개선안 참고).
 */
t_cd_err	check_error(char **argc)
{
	t_cd_err	err;
	int			i;

	i = 0;
	while (argc[i])
		i++;
	if (i > 2)
		err = CD_TOO_MANY_ARGS;
	else if (!argc[1] || (argc[1][0] == '-' && argc[1][1] == '-' &&
			argc[1][2] == 0))
		err = CD_HOME_NOT_SET;
	else if (argc[1][0] == '-')
		err = CD_HOME_NOT_SET;
	else if (errno == EACCES)
		err = CD_EACCES;
	else if (errno == ENOENT)
		err = CD_ENOENT;
	else if (errno == ENOTDIR)
		err = CD_ENOTDIR;
	else
		err = CD_UNKNOWN;
	return (err);
}

/**
 * @brief cd 실패 시 상태 코드 설정 및 사용자 메시지 출력.
 *
 * @param err     cd 에러 타입
 * @param new_pwd 시도한 대상 경로(메시지용, NULL 가능)
 * @param old_pwd 이전 작업 디렉터리(해제 필요 시 전달)
 * @return t_status 항상 FAILURE
 *
 * @note
 * - bash 규약에 맞춰 실패 시 보통 종료 상태는 1,
 *   "too many arguments"는 2를 사용한다.
 * - 출력은 표준에러로 보내는 것이 일반적이다.
 */
t_status	cd_fail(t_cd_err err, char *new_pwd, char *old_pwd)
{
	if (err == CD_TOO_MANY_ARGS)
	{
		get_shell_data()->last_status = 2;
		printf("bash: cd: too many arguments\n");
	}
	else
		get_shell_data()->last_status = 1;
	if (err == CD_HOME_NOT_SET)
		printf("bash: cd: HOME not set\n");
	if (err == CD_OLDPWD_NOT_SET)
		printf("cd: OLDPWD not set\n");
	if (err == CD_EACCES)
		printf("bash: cd: permission denied: %s\n", new_pwd);
	if (err == CD_ENOENT)
		printf("bash: cd: no such file or directory: %s\n", new_pwd);
	if (err == CD_ENOTDIR)
		printf("bash: cd: not a directory: %s\n", new_pwd);
	if (old_pwd)
		free(old_pwd);
	return (ERROR);
}

/**
 * @brief cd 명령어의 인자에 따라 이동할 경로를 결정한다.
 *
 * 이 함수는 cd 명령어 실행 시 입력된 인자와 환경 변수를 기반으로
 * 실제로 이동할 경로(new_pwd)를 선택한다.
 *
 * 동작 방식:
 * - 인자가 없거나 `"--"`만 입력된 경우: HOME 환경 변수 값으로 이동
 * - 빈 문자열("")이 입력된 경우: 현재 디렉토리(old_pwd) 유지
 * - `"-"`만 입력된 경우: 이전 디렉토리(OLDPWD 환경 변수)로 이동하고 경로를 출력
 * - 그 외의 경우: 입력된 경로(path)로 이동
 *
 * @param path     cd 명령어에 입력된 경로 인자
 * @param old_pwd  현재 디렉토리 경로
 * @param map      환경 변수를 저장한 해시 맵
 *
 * @return char*   이동할 대상 경로 문자열
 */
char	*route_set(char *path, char *old_pwd, t_hash_map *map)
{
	char	*new_pwd;

	if (!path || (path[0] == '-' && path[1] == '-' && path[2] == 0))
		new_pwd = get_value(map, "HOME");
	else if (path[0] == 0)
		new_pwd = old_pwd;
	else if (path[0] == '-' && path[1] == 0)
	{
		new_pwd = get_value(map, "OLDPWD");
		printf ("%s\n", new_pwd);
	}
	else
		new_pwd = path;
	return (new_pwd);
}

/**
 * @brief 현재 작업 디렉토리를 변경하고 환경 변수를 갱신한다.
 *
 * 이 함수는 `cd` 빌트인 명령어를 구현한 것으로,
 * 인자로 받은 경로로 작업 디렉토리를 변경하고 `PWD`와 `OLDPWD` 환경 변수를
 * 셸의 환경 변수 맵에 반영한다.
 *
 * 동작 방식:
 * - 인자가 2개 이상일 경우 오류 메시지를 출력하고 동작을 중단한다.
 * - 대상 경로를 찾지 못하거나 접근할 수 없으면 오류 메시지를 출력하고 종료한다.
 * - 성공 시 이전 경로를 "OLDPWD"에, 새로운 경로를 "PWD"에 저장한다.
 *
 * @param argc  명령어와 인자가 담긴 문자열 배열
 *              - argc[0] : "cd"
 *              - argc[1] : 이동할 경로 (선택 사항)
 *              - argc[2] : 존재할 경우 인자 초과 오류 발생
 *
 * @return t_status
 *         - SUCCESS: 디렉토리 변경과 환경 변수 갱신 성공
 *         - FAILURE: 경로 오류, 접근 권한 없음 등으로 실패
 */
t_status	builtin_cd(char **argc)
{
	t_hash_map	*envp_map;
	char		*old_pwd;
	char		*new_pwd;
	char		*cwd;

	envp_map = &get_shell_data()->envp_map;
	old_pwd = getcwd(NULL, 0);
	new_pwd = route_set(argc[1], old_pwd, envp_map);
	if (count_argc(argc) > 2 || !new_pwd || chdir(new_pwd) != 0)
		return (cd_fail(check_error(argc), argc[1], old_pwd));
	if (old_pwd)
		put_key_value(envp_map, "OLDPWD", old_pwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
		put_key_value(envp_map, "PWD", cwd);
	else
		put_key_value(envp_map, "PWD", ".");
	if (cwd)
		free (cwd);
	if (old_pwd)
		free (old_pwd);
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}
