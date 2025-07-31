/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:03:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 02:10:29 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"

static t_boolean	is_delim(char c);

char	*extract_normal_word(char *str)
{
	return (ft_substr(str, 0, find_next_token_delim(str) - str));
}

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
