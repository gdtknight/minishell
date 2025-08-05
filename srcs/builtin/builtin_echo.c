/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:18:39 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/04 20:42:21 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "libft.h"
#include "hashmap.h"
#include "builtin.h"

/**
 * @brief 전달받은 문자열을 출력한다.
 *
 * 전달받은 문자열 line을 표준 출력(stdout)에 출력하며,
 * flag 값에 따라 개행 문자를 추가한다.
 *
 * @param line  출력할 문자열
 * @param flag  OFF일 경우 개행 추가, ON일 경우 개행 생략
 */
void	builtin_echo(char *line, int flag)
{
	if (line)
		printf("%s", line);
	if (flag == OFF)
		printf("\n");
}

