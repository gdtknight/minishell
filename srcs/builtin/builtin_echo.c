/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:18:39 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/07 20:11:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief 전달받은 문자열을 출력한다.
 *
 * 전달받은 문자열 line을 표준 출력(stdout)에 출력하며,
 * flag 값에 따라 개행 문자를 추가한다.
 *
 * @param line  출력할 문자열
 * @param flag  OFF일 경우 개행 추가, ON일 경우 개행 생략
 */
t_status	builtin_echo(char *line, t_toggle toggle)
{
	if (line)
		printf("%s", line);
	if (toggle == OFF)
		printf("\n");
	return (SUCCESS);
}
