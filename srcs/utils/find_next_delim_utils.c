/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:46:34 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/19 22:00:39 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "debug.h"

t_boolean	is_ifs(char c)
{
	static const char	*ifs = " \t\n";
	char				*cur;

	cur = (char *)ifs;
	while (*cur)
		if (c == *cur++)
		{
			debug("find ifs!! : \'%c\'", c);
			return (TRUE);
		}
	return (FALSE);
}

t_boolean	is_dollar_sign(char c)
{
	return (c == '$');
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
t_boolean	is_tk_delim(char c)
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

t_boolean	is_single_quote(char c)
{
	return (c == '\'');
}
