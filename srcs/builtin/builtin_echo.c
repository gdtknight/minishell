/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:18:39 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/15 20:36:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "def.h"

#include "builtin.h"
#include "shell_data.h"

static int	check_option(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		i++;
	}
	return (i);
}

/**
 * @brief echo 빌트인 명령어를 실행한다.
 *
 * 이 함수는 `echo` 명령어를 구현한 것으로, 인자를 공백으로 구분하여 출력한다.
 * 옵션 `-n`이 없는 경우 출력 후 개행 문자를 추가한다.
 *
 * 동작 방식:
 * - 인자가 없는 경우: 개행 문자만 출력하고 종료
 * - 인자가 있는 경우:
 *   - `check_option()`을 통해 `-n` 옵션 여부를 확인
 *   - 옵션 이후의 모든 인자를 공백으로 구분하여 출력
 *   - `-n` 옵션이 없으면 마지막에 개행 문자 출력
 * - 실행이 끝나면 `last_status`를 0으로 설정
 *
 * @param args  Null-terminated 문자열 배열
 *              - args[0] : "echo"
 *              - args[1..n] : 출력할 문자열 및 옵션
 *
 * @return t_status
 *         - SUCCESS: 항상 성공 반환
 */
t_status	builtin_echo(char **args)
{
	int	print_idx;
	int	i;

	if (!args[1])
	{
		get_shell_data()->last_status = 0;
		printf("\n");
		return (SUCCESS);
	}
	print_idx = check_option(args);
	i = print_idx;
	printf("%s", args[i++]);
	while (args[i])
	{
		printf(" %s", args[i]);
		i++;
	}
	if (print_idx == 1)
		printf("\n");
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}
