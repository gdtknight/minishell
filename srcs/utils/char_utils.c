/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:46:34 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 21:12:23 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file char_utils.c
 * @brief Character classification helpers for minishell.
 *
 * This file provides helper functions to check whether a character
 * belongs to a specific category such as IFS (Internal Field Separator),
 * token delimiters, quotes, or special symbols used in the shell.
 */

#include "utils.h"

/**
 * @brief Check if a character is an IFS (Internal Field Separator).
 *
 * By default, IFS characters are space, tab, and newline.
 *
 * @param c Character to check.
 * @return TRUE if the character is an IFS, FALSE otherwise.
 */
t_boolean	is_ifs(char c)
{
	static const char	*ifs = " \t\n";
	char				*cur;

	cur = (char *)ifs;
	while (*cur)
	{
		if (c == *cur++)
			return (TRUE);
	}
	return (FALSE);
}

/**
 * @brief Check if a character is a dollar sign (`$`).
 *
 * Used to detect variable expansion markers in the shell.
 *
 * @param c Character to check.
 * @return TRUE if the character is '$', FALSE otherwise.
 */
t_boolean	is_dollar_sign(char c)
{
	return (c == '$');
}

/**
 * @brief Check if a character is a token delimiter.
 *
 * Recognized delimiters include:
 * - Whitespace (space, tab)
 * - Pipe (`|`)
 * - Ampersand (`&`)
 * - Semicolon (`;`)
 * - Redirection operators (`<`, `>`)
 * - Parentheses (`(`, `)`)
 *
 * @param c Character to check.
 * @return TRUE if the character is a delimiter, FALSE otherwise.
 */
t_boolean	is_tk_delim(char c)
{
	static char	tk_delims[] = {
		' ', '\t', '\n', '\r', '|', '&', ';',
		'<', '>', '(', ')', '\0',
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

/**
 * @brief Check if a character is a single quote (`'`).
 *
 * Used to track quoting state in parsing.
 *
 * @param c Character to check.
 * @return TRUE if the character is a single quote, FALSE otherwise.
 */
t_boolean	is_single_quote(char c)
{
	return (c == '\'');
}

t_boolean	is_space(int c)
{
	if (c == ' ' || c == '\f'
		|| c == '\v' || c == '\t' || c == '\r')
		return (TRUE);
	return (FALSE);
}
