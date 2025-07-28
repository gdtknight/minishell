/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:03:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 14:43:30 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"

static char	*find_next_delim(char *cur, char delim);
static char	*find_next_token_delim(char *cur);

/*
 * char *str = "'cmd1 -option1 -option2'" 
 * 작은 따옴표로 둘러쌓인 문자열 추출
 */
char	*extract_squote_word(char *str)
{
	return (ft_substr(str, 1, \
			find_next_delim(str + 1, '\'') - (str + 1)));
}

/*
 * char *str = "\"cmd1 -option1 -option2\""
 * 큰 따옴표로 둘러쌓인 문자열 추출
 */
char	*extract_dquote_word(char *str)
{
	return (ft_substr(str, 1, \
			find_next_delim(str + 1, '\"') - (str + 1)));
}

char	*extract_normal_word(char *str)
{
	return (ft_substr(str, 0, find_next_token_delim(str + 1) - str));
}

static char	*find_next_delim(char *cur, char delim)
{
	char	m_flag;

	m_flag = 0;
	while (*cur)
	{
		if (!(m_flag & C_BACKSLASH))
		{
			if (*cur == '\\')
				m_flag ^= C_BACKSLASH;
			else if (*cur == delim)
				return (cur);
		}
		else
			m_flag ^= C_BACKSLASH;
		cur++;
	}
	return (cur);
}

static char	*find_next_token_delim(char *cur)
{
	static char	tk_delims[] = {
		' ', '\t', '|', '&', ';',
		'<', '>', '(', ')', '\0'
	};
	char		*delim;
	char		m_flag;

	m_flag = 0;
	while (*cur)
	{
		if (!(m_flag & C_BACKSLASH))
		{
			if (*cur == '\\')
				m_flag ^= C_BACKSLASH;
			else
			{
				delim = tk_delims;
				while (*delim)
				{
					if (*cur == *delim)
						return (cur);
					else
						delim++;
				}
			}
		}
		else
			m_flag ^= C_BACKSLASH;
		cur++;
	}
	return (cur);
}
