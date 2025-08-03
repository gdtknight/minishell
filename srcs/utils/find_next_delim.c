/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 02:00:47 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/03 18:59:49 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "flag.h"

#include "def.h"

static t_boolean	update_flag(char c, char *flag, char mask);

/**
 * @brief 주어진 조건을 만족하는 다음 구분자 문자를 찾습니다.
 *
 * 문자열을 순회하면서 인용 부호 상태(싱글, 더블)와 백슬래시 상태를 추적하며,
 * 현재 문자가 마스크된 플래그에 해당하지 않는 상태이고 `predicate` 조건을 만족하면 해당 위치를 반환합니다.
 *
 * @param str 문자열의 시작 주소
 * @param predicate 구분자 조건을 검사하는 함수 포인터
 * @param mask 무시할 인용 상태를 지정하는 플래그 마스크
 * @return 조건을 만족하는 구분자 문자의 위치를 가리키는 포인터
 */
char	*find_next_delim(char *str, t_boolean (*predicate)(char), unsigned char mask)
{
	char	flag;

	flag = 0;
	while (*str)
	{
		if (update_flag(*str, &flag, mask))
			str++;
		if ((flag & mask) == 0 && predicate(*str))
			return (str);
		str++;
	}
	return (str);
}

/**
 * @brief 플래그 상태를 갱신합니다.
 *
 * 주어진 문자 `c`를 기반으로 현재 인용 부호나 백슬래시 상태를 추적합니다.
 * `mask`에 지정된 플래그들에 대해서만 상태를 변경합니다.
 *
 * @param c 현재 처리 중인 문자
 * @param flag 플래그 상태를 저장하는 변수의 포인터
 * @param mask 체크할 플래그 마스크 (예: C_SQUOTE, C_DQUOTE, C_BACKSLASH)
 * @return 플래그 상태가 변경되었으면 TRUE, 변경되지 않았으면 FALSE
 */
static t_boolean	update_flag(char c, char *flag, char mask)
{
	char	old_flag;

	old_flag = *flag;
	if (!(*flag & C_BACKSLASH))
	{
		if ((mask & C_BACKSLASH) && c == '\\')
			*flag |= C_BACKSLASH;
		else if ((mask & C_SQUOTE) && !(*flag & C_DQUOTE) && c == '\'')
			*flag ^= C_SQUOTE;
		else if ((mask & C_DQUOTE) && !(*flag & C_SQUOTE) && c == '"')
			*flag ^= C_DQUOTE;
	}
	else
		*flag &= ~C_BACKSLASH;
	return (*flag != old_flag);
}
