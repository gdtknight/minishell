/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:03:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/13 19:16:13 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "flag.h"

#include "tokenizer.h"

static t_boolean	is_delim(char c);

/**
 * @brief 일반 단어 토큰 문자열을 추출한다.
 *
 * 현재 문자열(`str`)에서 다음 토큰 구분자 전까지의 부분 문자열을 잘라서 반환한다.
 * 인용부호, 백슬래시 등의 상태는 find_next_token_delim()에서 처리된다.
 *
 * @param str 원본 문자열 (현재 토큰 시작 위치)
 * @return char* 추출된 부분 문자열(동적 할당)
 *
 * @note
 * - 반환된 문자열은 호출자가 free()로 해제해야 한다.
 * - 내부적으로 ft_substr()를 사용하므로 메모리 할당 실패 시 NULL을 반환할 수 있다.
 */
char	*extract_normal_word(char *str)
{
	return (ft_substr(str, 0, find_next_token_delim(str) - str));
}

/**
 * @brief 현재 위치에서 다음 토큰 구분자의 위치를 찾는다.
 *
 * 인용부호(`'`, `"`)나 백슬래시(`\`) 상태를 고려하여, 실제 토큰이 끝나는
 * 지점을 찾아 그 위치의 포인터를 반환한다.
 *
 * @param cur 탐색을 시작할 문자열 포인터
 * @return char* 다음 토큰 구분자 위치(구분자 문자를 가리킴)
 * @retval cur 문자열 끝까지 구분자를 찾지 못한 경우
 *
 * @note
 * - C_SQUOTE, C_DQUOTE, C_BACKSLASH 플래그를 사용해 인용/이스케이프 상태를 추적한다.
 * - 구분자는 is_delim()으로 판별한다.
 * - 싱글/더블쿼트 안에서는 구분자를 무시한다.
 */
char	*find_next_token_delim(char *cur)
{
	char		m_flag;

	m_flag = 0;
	while (*cur)
	{
		if (!(m_flag & C_BACKSLASH))
		{
			if (*cur == '\\')
				m_flag ^= C_BACKSLASH;
			else if (*cur == '\'')
				m_flag ^= C_SQUOTE;
			else if (*cur == '\"')
				m_flag ^= C_DQUOTE;
			else if (!(m_flag & C_SQUOTE) && !(m_flag & C_DQUOTE)
				&& !(m_flag & C_BACKSLASH) && is_delim(*cur))
				return (cur);
		}
		else
			m_flag ^= C_BACKSLASH;
		cur++;
	}
	return (cur);
}

/**
 * @brief 토큰 구분자인지 여부를 판별한다.
 *
 * 공백, 탭, 파이프('|'), 앰퍼샌드('&'), 세미콜론(';'),
 * 리다이렉션 기호('<', '>'), 괄호('(', ')')를 구분자로 인식한다.
 *
 * @param c 판별할 문자
 * @return t_boolean TRUE(구분자), FALSE(아님)
 */
static t_boolean	is_delim(char c)
{
	static char	tk_delims[] = {
		' ', '\t', '|', '&', ';',
		'<', '>', '(', ')', '\0'
	};
	int			idx;

	idx = 0;
	while (tk_delims[idx] != '\0')
	{
		if (c == tk_delims[idx++])
			return (TRUE);
	}
	return (FALSE);
}
