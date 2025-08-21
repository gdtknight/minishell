/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:44:13 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/21 10:10:55 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

#include "def.h"
#include "builtin.h"
#include "shell.h"

static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\v' || c == '\t' || c == '\r')
		return (1);
	return (0);
}

/**
 * @brief 문자열을 long long으로 변환한다.
 *
 * 공백, 부호(+/-)를 처리하며, 숫자가 아닌 문자가 포함되거나
 * 오버플로우가 발생하면 flag_atoll을 FALSE로 설정한다.
 *
 * @param nptr       변환할 문자열
 * @param flag_atoll 변환 성공 여부 플래그 포인터
 * @return 변환된 long long 값 (실패 시 값은 의미 없음)
 */
static long long	ft_atoll(const char *nptr, t_builtin_exit *flag_args)
{
	int			flag;
	long long	result;
	long long	temp;

	flag = 1;
	result = 0;
	if (!nptr)
		return (result);
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
		if (*nptr++ == '-')
			flag = -1;
	while (ft_isdigit(*nptr))
	{
		temp = result;
		result *= 10;
		result += (*nptr++) - '0';
		if (temp > result)
			*flag_args = WRONG_ARGC;
	}
	return ((result) * (flag));
}

/**
 * @brief exit 명령어 인자 유효성을 검사한다.
 *
 * @param args 명령어 인자 배열
 * @return t_builtin_exit
 *         - NO_ARGC        : 인자 없음
 *         - WITH_ARGC      : 올바른 숫자 인자 1개
 *         - WRONG_ARGC     : 숫자가 아닌 인자
 *         - TOO_MANY_ARGC  : 인자 2개 이상
 */
static t_builtin_exit	check_args(char **args)
{
	int	i;
	int	j;

	i = 1;
	if (!args[i])
		return (NO_ARGC);
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (!ft_isdigit(args[i][j]))
				return (WRONG_ARGC);
			j++;
		}
		i++;
	}
	if (i > 2)
		return (TOO_MANY_ARGC);
	return (WITH_ARGC);
}

/**
 * @brief exit 인자 오류 메시지를 출력한다.
 *
 * @param flag_args 인자 검사 결과 플래그
 * @param args      명령어 인자 배열
 */
static void	handle_flag(
				t_builtin_exit flag_args,
				char **args,
				long long exit_code)
{
	get_shell_data()->is_exit = TRUE;
	if (flag_args == WITH_ARGC)
		get_shell_data()->last_status = (int)((exit_code) % 256);
	if (flag_args == WRONG_ARGC)
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		get_shell_data()->last_status = 2;
	}
	if (flag_args == TOO_MANY_ARGC)
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		get_shell_data()->last_status = 1;
		get_shell_data()->is_exit = FALSE;
	}
}

/**
 * @brief exit 빌트인 명령어를 실행한다.
 *
 * 이 함수는 셸을 종료하는 `exit` 명령어를 구현한다.
 * 인자 유효성 검사를 거쳐 종료 코드(exit status)를 설정하고,
 * 필요한 경우 에러 메시지를 출력한 후 프로그램을 종료한다.
 *
 * 동작 방식:
 * 1. 인자가 없는 경우: 마지막 명령어의 상태 코드(`last_status`)로 종료
 * 2. 인자가 숫자 하나인 경우:
 *    - 해당 값을 `long long`으로 변환하여 종료 코드로 사용 (0~255 범위, %256 적용)
 *    - 숫자가 아닌 문자가 포함되면 `numeric argument required` 에러 출력 후 종료 코드 2
 * 3. 인자가 2개 이상인 경우:
 *    - `too many arguments` 에러 출력 후 종료하지 않고 상태 코드 1 반환
 * 4. 종료 전 환경 변수 해시맵을 해제
 *
 * @param args Null-terminated 문자열 배열
 *             - args[0] : "exit"
 *             - args[1] : 종료 코드(선택)
 *             - args[2] : 존재 시 인자 초과 오류
 *
 * @return t_status
 *         - 인자가 많을 경우: 상태 코드 반환 (종료하지 않음)
 *         - 정상 종료 시: 반환 없이 프로그램 종료
 */
t_status	builtin_exit(char **args)
{
	t_builtin_exit	flag_args;
	long long		exit_code;

	ft_putstr_fd ("exit\n", STDOUT_FILENO);
	flag_args = check_args(args);
	exit_code = ft_atoll(args[1], &flag_args);
	handle_flag(flag_args, args, exit_code);
	return (SUCCESS);
}
