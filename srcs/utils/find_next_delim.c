/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 02:00:47 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 20:56:50 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file find_delim.c
 * @brief Utility functions for finding delimiter positions in strings.
 *
 * This file provides functions to locate delimiters in a string while
 * respecting quoting (`'`, `"`) and escaping (`\`) rules. It is mainly
 * used for tokenization and parsing in the shell.
 */

#include <limits.h>
#include <stdlib.h>

#include "def.h"
#include "flag.h"

static t_boolean	update_flag(char c, char *flag, char mask);

/**
 * @brief Find the next delimiter character in a string.
 *
 * Iterates through the string while keeping track of quoting
 * (single and double quotes) and escaping states. If the current
 * character satisfies the `predicate` condition and is not masked
 * by the quoting/escaping state, the pointer to that character is returned.
 *
 * @param str Input string.
 * @param predicate Function pointer that checks whether a character
 *        qualifies as a delimiter.
 * @param mask Bitmask specifying which states (e.g., C_SQUOTE, C_DQUOTE,
 *        C_BACKSLASH) should be ignored.
 * @return Pointer to the delimiter character if found,
 *         or pointer to the null terminator if not found,
 *         or NULL if `str` is NULL.
 */
char	*find_next_delim(char *str, t_boolean (*predicate)(char), char mask)
{
	char	flag;

	if (!str)
		return (NULL);
	flag = 0;
	while (*str)
	{
		if (update_flag(*str, &flag, mask))
		{
			str++;
			continue ;
		}
		if ((flag & mask) == 0 && predicate(*str))
			return (str);
		str++;
	}
	return (str);
}

/**
 * @brief Find the index of the next delimiter character in a string.
 *
 * Works like `find_next_delim()` but returns the position (index)
 * instead of a pointer. This is useful when index-based access is preferred.
 *
 * @param str Input string.
 * @param predicate Function pointer that checks whether a character
 *        qualifies as a delimiter.
 * @param mask Bitmask specifying which states (e.g., C_SQUOTE, C_DQUOTE,
 *        C_BACKSLASH) should be ignored.
 * @return Index of the delimiter character if found,
 *         length of the string if not found,
 *         or ULONG_MAX if `str` is NULL.
 */
size_t	find_next_delim_pos(char *str, t_boolean (*predicate)(char), char mask)
{
	size_t	idx;
	char	flag;

	if (!str)
		return (ULONG_MAX);
	flag = 0;
	idx = 0;
	while (str[idx])
	{
		if (update_flag(str[idx], &flag, mask))
		{
			idx++;
			continue ;
		}
		if ((flag & mask) == 0 && predicate(str[idx]))
			return (idx);
		idx++;
	}
	return (idx);
}

/**
 * @brief Update the parsing state flags based on the current character.
 *
 * Tracks quoting (`'`, `"`) and escaping (`\`) states while scanning a string.
 * The behavior depends on the `mask`, which specifies which states should be
 * considered. If the flag state changes, returns TRUE.
 *
 * @param c The current character being processed.
 * @param flag Pointer to the current flag state (modified in place).
 * @param mask Bitmask specifying which flags to check (e.g., C_SQUOTE,
 *        C_DQUOTE, C_BACKSLASH).
 * @return TRUE if the flag state changed, FALSE otherwise.
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
