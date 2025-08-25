/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 01:33:49 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 20:59:21 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file pair_validator.c
 * @brief Input validation utilities for checking balanced quotes and escapes.
 *
 * This file provides functions to verify whether an input string
 * contains valid pairs of quotes and escape sequences.
 */

#include "def.h"
#include "utils.h"

/**
 * @brief Check if a string has valid quote and escape pairs.
 *
 * This function scans the input string while tracking three states:
 * - Escape (`\`)
 * - Single quote (`'`)
 * - Double quote (`"`)
 *
 * Rules:
 * - A backslash outside of single quotes toggles escape state.
 * - A single quote toggles single-quote state, unless inside double quotes.
 * - A double quote toggles double-quote state, unless inside single quotes.
 *
 * At the end of the scan, if all states are closed (no unmatched quotes
 * or dangling escape), the input is considered valid.
 *
 * @param input The input string to validate.
 * @return TRUE if the string is valid (all pairs closed),
 *         FALSE if there are unclosed quotes or escape.
 */
t_boolean	is_valid_pair(char *input)
{
	t_boolean	state[3];
	char		c;

	state[IN_ESCAPE] = FALSE;
	state[IN_SQUOTE] = FALSE;
	state[IN_DQUOTE] = FALSE;
	while (*input)
	{
		c = *input++;
		if (state[IN_ESCAPE])
			state[IN_ESCAPE] = FALSE;
		else if (c == '\\' && !state[IN_SQUOTE])
			state[IN_ESCAPE] = TRUE;
		else if (c == '\'' && !state[IN_DQUOTE])
			state[IN_SQUOTE] = !state[IN_SQUOTE];
		else if (c == '"' && !state[IN_SQUOTE])
			state[IN_DQUOTE] = !state[IN_DQUOTE];
	}
	return (!state[IN_ESCAPE] && !state[IN_SQUOTE] && !state[IN_DQUOTE]);
}
