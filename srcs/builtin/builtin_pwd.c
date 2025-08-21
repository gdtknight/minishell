/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:35:10 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/20 18:11:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "builtin.h"
#include "shell.h"

/**
 * @brief 현재 작업 디렉토리 경로를 출력한다.
 *
 * getcwd 함수를 사용해 현재 작업 디렉토리의 절대 경로를 가져와
 * 표준 출력(stdout)에 출력한다.
 *
 * @return SUCCESS(출력 성공), FAIL(경로를 가져오지 못한 경우)
 */
t_status	builtin_pwd(char **args)
{
	char	*cwd;

	(void) args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ERROR);
	printf("%s\n", cwd);
	free(cwd);
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}
