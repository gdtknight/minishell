/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 22:12:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 10:23:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

#include "def.h"
#include "builtin.h"
#include "execute.h"

/**
 * @brief 빌트인(builtin) 명령어를 실행한다.
 *
 * 전달받은 명령어(cmd_form.cmd)가 등록된 빌트인 명령어 목록에 있는지 확인하고,
 * 일치하는 항목이 있으면 해당 빌트인 함수 포인터를 호출한다.
 *
 * @param cmd_form 실행할 명령의 구조체 (명령어 문자열, 인자 배열, 환경 변수 등)
 * @return t_status 빌트인 함수 실행 결과
 * @retval SUCCESS 빌트인 함수가 성공적으로 실행된 경우
 * @retval FAILURE 빌트인 명령어 목록에 해당 명령이 없는 경우
 *
 * @note
 * - builtins[] 배열에 명령어 문자열과 함수 포인터를 등록해 관리한다.
 * - 비교 시 ft_strncmp()로 명령어 문자열이 완전히 일치하는지 확인한다.
 * - 빌트인이 아닌 경우에는 외부 명령 실행 경로로 넘어가야 한다.
 */
void	execute_builtin(t_cmd_form *cmd_form)
{
	static t_builtin_entry	builtins[] = {
	{"cd", builtin_cd},
	{"export", builtin_export},
	{"pwd", builtin_pwd},
	{"unset", builtin_unset},
	{"echo", builtin_echo},
	{"env", builtin_env},
	{"exit", builtin_exit},
	};
	size_t					idx;

	idx = -1;
	while (++idx < sizeof(builtins) / sizeof(t_builtin_entry))
	{
		if (ft_strncmp(builtins[idx].cmd_str, \
				cmd_form->cmd, ft_strlen(builtins[idx].cmd_str) + 1) == 0)
		{
			(builtins[idx].cmd)(cmd_form->args);
			return ;
		}
	}
}

t_boolean	is_builtin(char *cmd)
{
	static char	*builtins[] = {
		"cd",
		"export",
		"pwd",
		"unset",
		"echo",
		"env",
		"exit",
	};
	size_t		idx;

	idx = -1;
	while (++idx < sizeof(builtins) / sizeof(char *))
	{
		if (ft_strncmp(builtins[idx], cmd, ft_strlen(cmd) + 1) == 0)
			return (TRUE);
	}
	return (FALSE);
}
